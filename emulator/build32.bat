@pushd %~dp0%

@set include=-I"libs\SDL2-2.26.4\i686-w64-mingw32\include\SDL2" -I"libs\SDL2_image-2.6.3\i686-w64-mingw32\include\SDL2" -I"libs\lua-5.3.6\include" -I"libs\wineditline-2.206\include"

@set compiler=%include% -Wall -pedantic -std=c++2a -O2

@set linker=-L"libs\SDL2-2.26.4\i686-w64-mingw32\lib" -L"libs\SDL2_image-2.6.3\i686-w64-mingw32\lib" -L"libs\lua-5.3.6" -L"libs\wineditline-2.206\lib32"
@set linker=%linker% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -llua53_32bit -ledit_static

@set files=src\casioemu.cpp src\Emulator.cpp src\Logger.cpp
@set files=%files% src\Chipset\CPU.cpp src\Chipset\CPUPushPop.cpp src\Chipset\MMURegion.cpp src\Chipset\CPUControl.cpp src\Chipset\CPUArithmetic.cpp src\Chipset\CPULoadStore.cpp src\Chipset\Chipset.cpp src\Chipset\MMU.cpp src\Chipset\InterruptSource.cpp
@set files=%files% src\Peripheral\BatteryBackedRAM.cpp src\Peripheral\Peripheral.cpp src\Peripheral\Keyboard.cpp src\Peripheral\Screen.cpp src\Peripheral\Timer.cpp src\Peripheral\StandbyControl.cpp src\Peripheral\ROMWindow.cpp src\Peripheral\Miscellaneous.cpp
@set files=%files% src\Gui\CodeViewer.cpp src\Gui\Command.cpp src\Data\ModelInfo.cpp
@set files=%files% src\Gui\imgui\imgui_impl_sdl2.cpp src\Gui\imgui\imgui_impl_sdlrenderer2.cpp src\Gui\imgui\imgui.cpp src\Gui\imgui\imgui_widgets.cpp src\Gui\imgui\imgui_tables.cpp src\Gui\imgui\imgui_draw.cpp

@set output_exe=casioemu32.exe

g++ %compiler% %files% %linker% -m32 -o %output_exe%

@popd