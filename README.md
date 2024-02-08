This is a **modified** version of the Casio emulator developed by [qiufuyu](https://github.com/qiufuyu123/CasioEmuX) and contains a nX/U8 disassembler written in lua and one written in cpp (their output formats are different).

Files are modified so that they can work on windows.

Note that ROMs are **not** included in the `models` folder (for copyright reasons), you have to obtain one from somewhere else or dump it from a real calculator or emulator. (note that models labeled with `_emu` are for ROMs dumped from official emulators)


# CasioEmuX-win

An emulator and disassembler for the CASIO calculator series using the nX-U8/100 core.  
With debuggers.

To build it, install tdm-gcc and run build.bat

syntax:

`casioemu key1=value1 key2=value2 ...`

`model=<directory>` model directory, which should contain interface.png, model.lua, rom.bin(you can find it elsewhere) and _disas.txt(use disas-cpp on rom.bin to obtain this file)

these arguments are optional:

`script=<xxx.lua>` the supplied lua script will run on startup

`exit_on_console_shutdown=true/false` pretty self-evident, default to false

`history=<history file path>` input history
