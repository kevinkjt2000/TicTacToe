SYS := $(shell g++ -dumpmachine)

OPTS = -c -O2 -g

ifneq (,$(findstring linux,$(SYS)))
	LIBS =
else
	ifneq (,$(findstring mingw,$(SYS)))
		LIBS = -lwsock32
	else
		ifneq (,$(findstring cygwin,$(SYS)))
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
	g++ -o Main.exe Main.o TicTacToe.o $(LIBS)

Main.o : Main.cpp
	g++ -o Main.o $(OPTS) Main.cpp

TicTacToe.o : TicTacToe.cpp
	g++ -o TicTacToe.o $(OPTS) TicTacToe.cpp

.PHONY : clean
clean : 
	rm -f *.o Main.exe
