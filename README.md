# CasioEmuX-win

This repo contains a **modified** version of the Casio emulator developed by [qiufuyu](https://github.com/qiufuyu123/CasioEmuX) and two nX/U8 disassemblers written in lua and written in cpp respectively (their output formats are different).

Files are modified so that they can work on windows.

Note that ROMs are **not** included in the `models` folder (for copyright reasons), you have to obtain one from somewhere else or dump it from a real calculator or emulator. (note that models labeled with `_emu` are for ROMs dumped from official emulators)

# Disassembler (lua)

Each argument should have one of these two formats:

* key=value.

* path: equivalent to input=path.

For the supported values of key: see args_assoc in disas.lua file.

# Disassembler (cpp)

## Build

Open msys2 mingw64 environment.

Run `pacman -S mingw-w64-x86_64-gcc` to install gcc.

Run `pacman -S mingw-w64-x86_64-make` to install `mingw32-make`.

Run `mingw32-make` in the `disas-cpp` directory.

If something goes wrong, run `mingw32-make clean` to clean up and start again.

## Usage

* `main` is for generating disassemblers, run it without arguments to show the help message.

* `u8-disas` is the nX/u8 disassembler, run it without arguments to show the help message.

* `u8-disas-split` splits 4-byte command into 2 2-byte commands, run it without arguments to show the help message

# Emulator

An emulator and disassembler for the CASIO calculator series using the nX-U8/100 core ported to windows.
With debuggers.

## Build

Open msys2 mingw64 environment.

Run `pacman -S mingw-w64-x86_64-gcc` to install gcc.

Run `pacman -S mingw-w64-x86_64-make` to install `mingw32-make`.

Run `mingw32-make` in the `emulator` directory.

If something goes wrong, run `mingw32-make clean` to clean up and start again.

## Command-line arguments

Each argument should have one of these two formats:

* `key=value` where `key` does not contain any equal signs.
* `path`: equivalent to `model=path`.

Supported values of `key` are: (if `value` is not mentioned then it does not matter)

* `paused`: Pause the emulator on start.
* `model`: Specify the path to model folder. Example `value`: `models/fx570esplus`.
* `ram`: Load RAM dump from the path specified in `value`.
* `clean_ram`: If `ram` is specified, this prevents the calculator from loading the file, instead starting from a *clean* RAM state.
* `preserve_ram`: Specify that the RAM should **not** be dumped (to the value associated with the `ram` key) on program exit, in other words, *preserve* the existing RAM dump in the file.
* `strict_memory`: Print an error message if the program attempt to write to unwritable memory regions corresponding to ROM. (writing to unmapped memory regions always print an error message)
* `pause_on_mem_error`: Pause the emulator when a memory error message is printed.
* `history`: Path to a file to load/save command history.
* `script`: Specify a path to Lua file to be executed on program startup (using `value` parameter).
* `width`, `height`: Initial calculator window width/height on program start. The values can be in hexadecimal (prefix `0x`), octal (prefix `0`) or decimal. The debugger window is hardcoded as 900x600.
* `exit_on_console_shutdown`: Exit the emulator when the console thread is shut down.

## Available Lua functions

Those Lua functions and variables can be used at the Lua prompt of the emulator.

* `emu:set_paused(-)`: Set emulator state. Called with a boolean value.
* `emu:tick()`: Execute one command.
* `emu:shutdown()`: Shutdown the emulator.

* `cpu.xxx`: Get register value. `xxx` should be one of
	* `r0` to `r15`
	* One of the register names. See `register_record_sources` array in `emulator\src\Chipset\CPU.cpp`.
	* `erN`, `xrN`, `qrN` are **not** supported.
* `cpu.bt`: A string containing the current stack trace.

* `code[address]`: Access code. (By words, only use even address, otherwise program will panic)
* `data[address]`: Access data. (By bytes)
* `data:watch(offset, fn)`: Set watchpoint at address `offset` - `fn` is called whenever
data is written to. If `fn` is `nil`, clear the watchpoint.
* `data:rwatch(offset, fn)`: Set watchpoint at address `offset` - `fn` is called whenever
data is read from as data. If `fn` is `nil`, clear the watchpoint.

Some additional functions are available in `lua-common.lua` file.
To use those, it's necessary to pass the flag `script=emulator/lua-common.lua`.
