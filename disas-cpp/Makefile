# folder names 'bin' and 'obj' are hardcoded
# tdm-gcc

CCFLAGS = -D_GLIBCXX_DEBUG -O2 -std=c++14 -Wall -Wextra -Werror -pedantic
CCFLAGS2 = -D_GLIBCXX_DEBUG -O2 -std=c++14 -Wall -Wextra -pedantic

all: u8-disas u8-disas-split

u8-disas: nX-U8_is.txt main Makefile example.cpp
	main $< temp.cpp
	g++ $(CCFLAGS) temp.cpp -o $@
	del temp.cpp

u8-disas-split: nX-U8_is_split.txt main Makefile example.cpp
	main $< temp.cpp
	g++ $(CCFLAGS) temp.cpp -o $@
	del temp.cpp

u8-disas-brief: nX-U8_brief.txt main Makefile example.cpp
	main $< temp.cpp
	g++ $(CCFLAGS2) temp.cpp -o $@
	del temp.cpp

main: main.cpp lib.h lib.cpp
	g++ $(CCFLAGS) main.cpp lib.cpp -o $@

clean:
	if exist main.exe del main.exe
	if exist u8-disas.exe del u8-disas.exe
	if exist u8-disas-split.exe del u8-disas-split.exe
	if exist u8-disas-split.exe del u8-disas-brief.exe
	if exist temp.cpp del temp.cpp
