#include "Watcher.hpp"
#include "../Chipset/CPU.hpp"
#include "../Chipset/Chipset.hpp"
#include "../Peripheral/BatteryBackedRAM.hpp"
#include "imgui/imgui.h"
#include <cstdint>
#include <cstdio>

void Watcher::DrawWindow() {
    ImGui::Begin("Watcher");
    ImGui::Text("Stack Trace");
    ImGui::BeginChild("##stack_trace", ImVec2(0, 8 * ImGui::GetTextLineHeight()));
    casioemu::Chipset &chipset = emu->chipset;
    casioemu::CPU &cpu = chipset.cpu;
    std::string s = cpu.GetBacktrace();
    std::string run_mode;
    if (chipset.run_mode == casioemu::Chipset::RM_STOP) {
        run_mode = "RM_STOP";
    } else if (chipset.run_mode == casioemu::Chipset::RM_HALT) {
        run_mode = "RM_HALT";
    } else { // RM_RUN
        run_mode = "RM_RUN";
    }
    ImGui::InputTextMultiline("##as", (char *)s.c_str(), s.size(), ImVec2(ImGui::GetWindowWidth(), 0), ImGuiInputTextFlags_ReadOnly);
    ImGui::EndChild();
    ImGui::Text("Registers");
    ImGui::BeginChild("##registers");
    ImGui::Text("r0  %02X | r1  %02X | r2  %02X | r3  %02X | PSW   %02X | LR   %01X:%04X", cpu.reg_r[ 0] & 0xff, cpu.reg_r[ 1] & 0xff, cpu.reg_r[ 2] & 0xff, cpu.reg_r[ 3] & 0xff, cpu.reg_psw     & 0xff, cpu.reg_lcsr    & 0xf, cpu.reg_lr     & 0xffff);
    ImGui::Text("r4  %02X | r5  %02X | r6  %02X | r7  %02X | EPSW1 %02X | ELR1 %01X:%04X", cpu.reg_r[ 4] & 0xff, cpu.reg_r[ 5] & 0xff, cpu.reg_r[ 6] & 0xff, cpu.reg_r[ 7] & 0xff, cpu.reg_epsw[1] & 0xff, cpu.reg_ecsr[1] & 0xf, cpu.reg_elr[1] & 0xffff);
    ImGui::Text("r8  %02X | r9  %02X | r10 %02X | r11 %02X | EPSW2 %02X | ELR2 %01X:%04X", cpu.reg_r[ 8] & 0xff, cpu.reg_r[ 9] & 0xff, cpu.reg_r[10] & 0xff, cpu.reg_r[11] & 0xff, cpu.reg_epsw[2] & 0xff, cpu.reg_ecsr[2] & 0xf, cpu.reg_elr[2] & 0xffff);
    ImGui::Text("r12 %02X | r13 %02X | r14 %02X | r15 %02X | EPSW3 %02X | ELR3 %01X:%04X", cpu.reg_r[12] & 0xff, cpu.reg_r[13] & 0xff, cpu.reg_r[14] & 0xff, cpu.reg_r[15] & 0xff, cpu.reg_epsw[3] & 0xff, cpu.reg_ecsr[3] & 0xf, cpu.reg_elr[3] & 0xffff);
    ImGui::Text("SP %04X, EA %04X, ELVL %01X, PC %01X:%04X, %s", cpu.reg_sp & 0xffff, cpu.reg_ea & 0xffff, cpu.reg_psw & 3, cpu.reg_csr & 0xf, cpu.reg_pc & 0xffff, run_mode.c_str());
    ImGui::EndChild();
    ImGui::End();
}
