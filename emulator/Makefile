SRCDIR += \
	./src \
	./src/Chipset \
	./src/Data \
	./src/Gui \
	./src/Gui/imgui \
	./src/Peripheral

VPATH = $(SRCDIR)

sources := $(sort $(shell find $(SRCDIR) -name \*.cpp))
objects := $(patsubst %.cpp, obj/%.o, $(notdir $(sources)))

_dummy := $(shell mkdir -p obj)

all: $(objects)
	g++ -L ./lib $(objects) -static-libgcc -static-libstdc++ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -llua53 -lreadline -lhistory -ltermcap -Wl,-Bstatic -lstdc++ -lpthread -o casioemu.exe

obj/%.o: %.cpp
	g++ -O2 -std=c++14 -Wall -Wextra -Werror -pedantic -I ./include -c $< -o $@

.PHONY: clean
clean:
	rm -f obj/*
	rm -f casioemu.exe
