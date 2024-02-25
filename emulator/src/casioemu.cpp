#include "Config.hpp"
#include "Gui/imgui/imgui_impl_sdl2.h"
#include "Gui/ui.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>

#include "Data/EventCode.hpp"
#include "Emulator.hpp"
#include "Logger.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include "SDL_video.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <editline/readline.h>

using namespace casioemu;

int main(int argc, char *argv[]) {
    std::map<std::string, std::string> argv_map;
    for (int ix = 1; ix != argc; ++ix) {
        std::string key, value;
        char *eq_pos = strchr(argv[ix], '=');
        if (eq_pos) {
            key = std::string(argv[ix], eq_pos);
            value = eq_pos + 1;
        } else {
            key = "model";
            value = argv[ix];
        }

        if (argv_map.find(key) == argv_map.end())
            argv_map[key] = value;
        else
            logger::Info("[argv] #%i: key '%s' already set\n", ix, key.c_str());
    }

    if (argv_map.find("model") == argv_map.end()) {
        printf("No model path supplied\n");
        exit(2);
    }

    int sdlFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    if (SDL_Init(sdlFlags) != 0)
        PANIC("SDL_Init failed: %s\n", SDL_GetError());

    int imgFlags = IMG_INIT_PNG;
    if (IMG_Init(imgFlags) != imgFlags)
        PANIC("IMG_Init failed: %s\n", IMG_GetError());

    std::string history_filename;
    auto history_filename_iter = argv_map.find("history");
    if (history_filename_iter != argv_map.end())
        history_filename = history_filename_iter->second;

    if (!history_filename.empty()) {
        int err = read_history(history_filename.c_str());
        if (err && err != ENOENT)
            PANIC("error while reading history file: %s\n", std::strerror(err));
    }
    
    static std::atomic<bool> got_inp(false);
    {
        Emulator emulator(argv_map);
        m_emu = &emulator;

        // Note: argv_map must be destructed after emulator.

        // Used to signal to the console input thread when to stop.
        static std::atomic<bool> running(true);
        static std::atomic<int> inp_cnt(0);
        static char pr[500];

        test_gui();
        std::thread console_input_thread([&] {
            while (1) {
                char *console_input_c_str;
                got_inp = false;
                std::thread readline_thread([&] {
                    sprintf(pr, "In [%d]: ", ++inp_cnt);
                    console_input_c_str = readline(pr);
                    got_inp = true;
                });
                readline_thread.detach();

                while (!got_inp)
                    if (!running)
                        return;

                if (console_input_c_str == NULL) {
                    if (argv_map.find("exit_on_console_shutdown") != argv_map.end()) {
                        SDL_Event event;
                        SDL_zero(event);
                        event.type = SDL_WINDOWEVENT;
                        event.window.event = SDL_WINDOWEVENT_CLOSE;
                        SDL_PushEvent(&event);
                    } else {
                        logger::Info("Console thread shutting down\n");
                    }
                    return;
                }

                // Ignore empty lines.
                if (console_input_c_str[0] == 0)
                    continue;

                add_history(console_input_c_str);

                std::lock_guard<decltype(emulator.access_mx)> access_lock(emulator.access_mx);
                if (!emulator.Running())
                    return;
                emulator.ExecuteCommand(console_input_c_str);
                free(console_input_c_str);

                if (!emulator.Running()) {
                    SDL_Event event;
                    SDL_zero(event);
                    event.type = SDL_USEREVENT;
                    event.user.code = CE_EMU_STOPPED;
                    SDL_PushEvent(&event);
                    return;
                }
            }
        });
        std::thread t1([&]() {
            while (1) {
                gui_loop();
            }
        });
        t1.detach();

        while (emulator.Running()) {
            SDL_Event event;
            if (!SDL_PollEvent(&event))
                continue;

            switch (event.type) {
            case SDL_USEREVENT:
                switch (event.user.code) {
                case CE_FRAME_REQUEST:
                    emulator.Frame();
                    break;
                case CE_EMU_STOPPED:
                    if (emulator.Running())
                        PANIC("CE_EMU_STOPPED event received while emulator is still running\n");
                    break;
                }
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    emulator.Shutdown();
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    if (event.window.windowID == SDL_GetWindowID(emulator.window)) {
                        emulator.WindowResize(event.window.data1, event.window.data2);
                    }
                    break;
                case SDL_WINDOWEVENT_EXPOSED:
                    emulator.Repaint();
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_TEXTINPUT:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
                if (SDL_GetKeyboardFocus() != emulator.window || SDL_GetMouseFocus() != emulator.window) {
                    ImGui_ImplSDL2_ProcessEvent(&event);
                    break;
                }
                emulator.UIEvent(event);
                break;
            }
        }

        running = false;
        console_input_thread.join();
    }

    if (!got_inp)
        std::cout << std::endl;
    std::cout << "Goodbye" << std::endl;

    IMG_Quit();
    SDL_Quit();

    if (!history_filename.empty()) {
        int err = write_history(history_filename.c_str());
        if (err)
            PANIC("error while writing history file: %s\n", std::strerror(err));
    }

    return 0;
}
