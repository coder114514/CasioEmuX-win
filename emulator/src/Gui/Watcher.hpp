#pragma once

#include "../Emulator.hpp"

class Watcher {
private:
    casioemu::Emulator* emu;
public:
    Watcher(casioemu::Emulator *_emu): emu(_emu) {}
    void DrawWindow();
};
