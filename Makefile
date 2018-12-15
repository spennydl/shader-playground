CC=clang++
CFLAGS=-W -Wall -std=c++17
OUTFILE=splay
CLIBS=-lSDL2 -lGL -lGLEW
CPP_SRC=main.cpp sdl_util.cpp

all:
	${CC} ${CFLAGS} ${CLIBS} ${CPP_SRC} -o ${OUTFILE}