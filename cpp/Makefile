SYS := $(shell $(CXX) -dumpmachine)

OPTS = -std=c++11 -c -O2 -g -Wall -Wextra -Werror -pedantic -pedantic-errors

ifneq (,$(findstring linux,$(SYS)))
	LIBS =
else
	ifneq (,$(findstring mingw,$(SYS)))
		OPTS += -DWIN32
		LIBS = -lwsock32
	else
		ifneq (,$(findstring cygwin,$(SYS)))
			OPTS += -DWIN32
			LIBS = -lwsock32
		else
			LIBS =
		endif
	endif
endif

.PHONY : all
all : Main.exe

.PHONY : run
run : all
	./Main.exe 127.0.0.1 10020

Main.exe : Main.o TicTacToe.o
	$(CXX) -o Main.exe Main.o TicTacToe.o $(LIBS)

Main.o : Main.cpp
	$(CXX) -o Main.o $(OPTS) Main.cpp

TicTacToe.o : TicTacToe.cpp TicTacToe.h
	$(CXX) -o TicTacToe.o $(OPTS) TicTacToe.cpp

.PHONY : clang-format
clang-format:
	for f in *.cpp *.h; do clang-format $$f | sponge $$f; done

.PHONY : clean
clean :
	rm -f *.o Main.exe
