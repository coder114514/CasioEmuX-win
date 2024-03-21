#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct CodeElem {
    uint8_t segment;
    uint16_t offset;
    char srcbuf[40];
    CodeElem() {}
    CodeElem(uint8_t seg, uint16_t off) : segment(seg), offset(off) {}
};

size_t get_real_pc(const CodeElem&);
size_t get_real_pc(uint8_t, uint16_t);

enum EmuDebugFlag {
    DEBUG_BREAKPOINT = 1,
    DEBUG_STEP = 2,
    DEBUG_RET_TRACE = 4
};

class CodeViewer {
private:
    std::map<int, uint8_t> break_points;
    std::vector<CodeElem> codes;
    size_t rows;
    std::string src_path;
    char adrbuf[9]{0};
    int max_row = 0;
    int max_col = 0;
    int cur_row = 0;

    bool is_loaded = false;
    bool need_roll = false;
    int64_t triggered_bp_line = -1;

public:
    uint8_t debug_flags = DEBUG_BREAKPOINT;
    CodeViewer(std::string path);
    ~CodeViewer();
    bool TryTrigBP(uint8_t seg, uint16_t offset, bool bp_mode = true);
    CodeElem LookUp(uint8_t seg, uint16_t offset, int *idx = nullptr);
    void DrawWindow();
    void DrawContent();
    void DrawMonitor();
    void JumpTo(uint8_t seg, uint16_t offset);
};
