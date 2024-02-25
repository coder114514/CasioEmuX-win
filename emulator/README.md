# Emulator

An emulator and disassembler for the CASIO calculator series using the nX-U8/100 core ported to windows.
With debuggers.

To build it, install tdm-gcc and run build32.bat or build64.bat (depending on your machine's architecture).

The dlls needed by the program (SDL2.dll, SDL2_image.dll) are in the `dlls` directory.

## Syntax

`casioemu <key1>=<value1> <key2>=<value2> ...`

`model=<directory>` model directory, which should contain interface.png, model.lua, rom.bin(you can find it elsewhere) and _disas.txt(use disas-cpp on rom.bin to obtain this file)

these arguments are optional:

`script=<xxx.lua>` the supplied lua script will run on startup

`exit_on_console_shutdown=true/false` pretty self-evident, default to false

`history=<history file path>` the file that stores your input history
