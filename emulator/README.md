# Emulator

An emulator and disassembler for the CASIO calculator series using the nX-U8/100 core ported to windows.
With debuggers.

building: use msys2 mingw64 env

## Options

#### mandatory

`model=<directory>` model directory, which should contain interface.png, model.lua, rom.bin(you can find it elsewhere) and _disas.txt(use disas-cpp on rom.bin to obtain this file)

#### optional:

`script=<lua script path>` the lua script that runs on startup

`exit_on_console_shutdown` shutdown the entire emulator when console reads EOF

`history=<history file path>` the file that stores input history

`width=<xxx>`, `height=<xxx>` the width of height of the calculator window

`paused` pause the emulator after startup

`pause_on_mem_error` pause the emulator when there's memory error

`ram=<ram iamge path>` the file that stores ram iamge

`preserve_ram` save the ram into the ram image file after exit

`clean_ram` do not load the ram image on startup

`strict_memory` treat writes to the rom as memory errors
