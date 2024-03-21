#pragma once
#include "../Emulator.hpp"
#include "CodeViewer.hpp"
#include "Watcher.hpp"

int init_debugger_window();
void debugger_gui_loop();
extern char *n_ram_buffer;
extern casioemu::Emulator *m_emu;
extern CodeViewer *code_viewer;
extern Watcher *watcher;
