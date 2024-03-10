g++ -Wall -pedantic -std=c++14 -O2 \
-I ./libs/SDL2-2.26.4/x86_64-w64-mingw32/include/SDL2 \
-I ./libs/SDL2_image-2.6.3/x86_64-w64-mingw32/include/SDL2 \
-I ./libs/lua-5.3.6/include \
-I ./libs/readline-8/include \
./src/casioemu.cpp ./src/Emulator.cpp ./src/Logger.cpp  ./src/Chipset/CPU.cpp ./src/Chipset/CPUPushPop.cpp ./src/Chipset/MMURegion.cpp ./src/Chipset/CPUControl.cpp ./src/Chipset/CPUArithmetic.cpp ./src/Chipset/CPULoadStore.cpp ./src/Chipset/Chipset.cpp ./src/Chipset/MMU.cpp ./src/Chipset/InterruptSource.cpp ./src/Peripheral/BatteryBackedRAM.cpp ./src/Peripheral/Peripheral.cpp ./src/Peripheral/Keyboard.cpp ./src/Peripheral/Screen.cpp ./src/Peripheral/Timer.cpp ./src/Peripheral/StandbyControl.cpp ./src/Peripheral/ROMWindow.cpp ./src/Peripheral/Miscellaneous.cpp ./src/Gui/CodeViewer.cpp ./src/Gui/ui.cpp ./src/Data/ModelInfo.cpp ./src/Gui/imgui/imgui_impl_sdl2.cpp ./src/Gui/imgui/imgui_impl_sdlrenderer2.cpp ./src/Gui/imgui/imgui.cpp ./src/Gui/imgui/imgui_widgets.cpp ./src/Gui/imgui/imgui_tables.cpp ./src/Gui/imgui/imgui_draw.cpp -o ./casioemu.exe \
-L"./libs/lua-5.3.6" \
-L"./libs/SDL2_image-2.6.3/x86_64-w64-mingw32/lib" \
-L"./libs/SDL2-2.26.4/x86_64-w64-mingw32/lib" \
-L"./libs/readline-8" \
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -llua53 -lreadline -lhistory

