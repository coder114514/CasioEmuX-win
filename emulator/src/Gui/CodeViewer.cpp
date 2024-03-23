#include "CodeViewer.hpp"
#include "../Chipset/CPU.hpp"
#include "../Chipset/Chipset.hpp"
#include "../Config.hpp"
#include "../Emulator.hpp"
#include "../Logger.hpp"
#include "imgui/imgui.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

casioemu::Emulator *m_emu = nullptr;

size_t get_real_pc(const CodeElem& e) {
    return get_real_pc(e.segment, e.offset);
}

size_t get_real_pc(uint8_t seg, uint16_t off) {
    return (seg << 16) | off;
}

CodeViewer::CodeViewer(std::string path) {
    src_path = path;
    std::ifstream f(src_path, std::ios::in);
    if (!f.is_open())
        PANIC("\nFail to open disassembly code src: %s\n", src_path.c_str());
    casioemu::logger::Info("Start to load disassembly ...\n");
    char buf[200], adr[6];
    while (!f.eof()) {
        memset(buf, 0, sizeof(buf));
        memset(adr, 0, sizeof(adr));
        f.getline(buf, 200);
        uint8_t seg = buf[1] - '0';
        uint8_t len = strlen(buf);
        if (!len)
            break;
        if (len > max_col)
            max_col = len;
        memcpy(adr, buf + 2, 4);
        uint16_t offset = std::stoi(adr, 0, 16);
        CodeElem e;
        e.offset = offset;
        e.segment = seg;
        memset(e.srcbuf, 0, sizeof(e.srcbuf));
        memcpy(e.srcbuf, buf + 28, len - 28);
        codes.push_back(e);
    }
    f.close();
    casioemu::logger::Info("Successfully loaded disassembly!\n");
    max_row = codes.size();
    is_loaded = true;
}

bool operator<(const CodeElem &a, const CodeElem &b) {
    return get_real_pc(a) < get_real_pc(b);
}

CodeViewer::~CodeViewer() {
}

CodeElem CodeViewer::LookUp(uint8_t seg, uint16_t offset, int *idx) {
    // binary search
    CodeElem target(seg, offset);
    auto it = std::upper_bound(codes.begin(), codes.end(), target);
    if (it != codes.begin())
        --it;
    if (idx)
        *idx = it - codes.begin();
    return CodeElem(it->segment, it->offset);
}

bool CodeViewer::TryTrigBP(uint8_t seg, uint16_t offset, bool is_bp) {
    if (!is_loaded) {
        return false;
    }
    if (!is_bp) { // step/trace
        int idx = 0;
        LookUp(seg, offset, &idx);
        cur_row = idx;
        triggered_bp_line = -1;
        try_roll = true;
        return true;
    }
    for (auto it = break_points.begin(); it != break_points.end(); it++) {
        if (it->second == 1) {
            CodeElem e = codes[it->first];
            if (e.segment == seg && e.offset == offset) {
                cur_row = it->first;
                triggered_bp_line = it->first;
                try_roll = true;
                return true;
            }
        }
    }
    return false;
}

void CodeViewer::DrawContent() {
    ImGuiListClipper c;
    c.Begin(max_row, ImGui::GetTextLineHeight());
    while (c.Step()) {
        for (int line_i = c.DisplayStart; line_i < c.DisplayEnd; line_i++) {
            CodeElem e = codes[line_i];
            auto it = break_points.find(line_i);
            if (line_i == triggered_bp_line) {
                // the break point is triggered!
                ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "[ > ]");
            } else if (it == break_points.end() || !break_points[line_i]) {
                ImGui::Text("[ o ]");
                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    break_points[line_i] = 1;
                }
            } else {
                ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "[ x ]");
                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    break_points[line_i] = 0;
                }
            }
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "%05zX", get_real_pc(e));
            ImGui::SameLine();
            if (m_emu->chipset.cpu.GetCurrentRealPC() == get_real_pc(e))
                ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", e.srcbuf);
            else
                ImGui::Text("%s", e.srcbuf);
        }
        if (try_roll) {
            try_roll = false;
            if (!(c.DisplayStart + 1 <= cur_row && cur_row < c.DisplayEnd - 5)) {
                float v = (float)cur_row / max_row * ImGui::GetScrollMaxY();
                ImGui::SetScrollY(v);
            }
        }
    }
}

static bool step_debug = false, trace_debug = false;

void CodeViewer::DrawWindow() {
    int h = ImGui::GetTextLineHeight() + 4;
    int w = ImGui::CalcTextSize("F").x;
    if (!is_loaded) {
        ImGui::SetNextWindowSize(ImVec2(w * 50, h * 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowContentSize(ImVec2(w * 50, h * 10));
        ImGui::Begin("Disassembly");
        ImGui::SetCursorPos(ImVec2(w * 2, h * 5));
        ImGui::Text("Loading...");
        ImGui::End();
        return;
    }
    ImGui::Begin("Disassembly", 0);
    ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetTextLineHeight() * 1.8f));
    DrawContent();
    ImGui::EndChild();
    ImGui::Text("Go to Addr:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::CalcTextSize("000000").x);
    if (ImGui::InputText("##input", adrbuf, sizeof(adrbuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        size_t addr;
        if (sscanf(adrbuf, "%zX", &addr) == 1)
            JumpTo(addr >> 16, addr & 0x0ffff);
    }
    ImGui::SameLine();
    ImGui::Checkbox("STEP", &step_debug);
    ImGui::SameLine();
    ImGui::Checkbox("TRACE", &trace_debug);
    if (m_emu->GetPaused()) {
        ImGui::SameLine();
        if (ImGui::Button("Continue")) {
            if (!step_debug && !trace_debug)
                break_points[triggered_bp_line] = 0;
            m_emu->SetPaused(false);
            triggered_bp_line = -1;
        }
    }
    ImGui::End();
    debug_flags = DEBUG_BREAKPOINT | (step_debug ? DEBUG_STEP : 0) | (trace_debug ? DEBUG_RET_TRACE : 0);
}

void CodeViewer::JumpTo(uint8_t seg, uint16_t offset) {
    int idx = 0;
    LookUp(seg, offset, &idx);
    cur_row = idx;
    try_roll = true;
}
