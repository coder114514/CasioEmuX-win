# folder names 'obj' are hardcoded

CXX ?= g++
RM  ?= rm -f
CCFLAGS ?= -D_GLIBCXX_DEBUG -O2 -std=c++14 -Wall -Wextra -Werror -pedantic

_dummy := $(shell mkdir -p obj)

all: u8-disas u8-disas-split

u8-disas: nX-U8_is.txt main Makefile example.cpp
	@main $< temp.cpp
	@$(CXX) $(CCFLAGS) temp.cpp -o $@
	@$(RM) temp.cpp

u8-disas-split: nX-U8_is_split.txt main Makefile example.cpp
	@main $< temp.cpp
	@$(CXX) $(CCFLAGS) temp.cpp -o $@
	@$(RM) temp.cpp

main: obj/main.o obj/lib.o Makefile
	@$(CXX) obj/main.o obj/lib.o -o $@

obj/%.o: %.cpp lib.h Makefile
	@$(CXX) -c $(CCFLAGS) $< -o $@

clean:
	@$(RM) main.exe
	@$(RM) u8-disas.exe
	@$(RM) u8-disas-split.exe
	@$(RM) obj/*
