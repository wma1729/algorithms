DBG     = -g
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = towersofhanoi.o skyline.o celebrity.o majority.o

EXES   = towersofhanoi skyline celebrity majority

all: ${EXES}

towersofhanoi: towersofhanoi.o
	g++ ${DBG} $^ -o $@

skyline: skyline.o
	g++ ${DBG} $^ -o $@

celebrity: celebrity.o
	g++ ${DBG} $^ -o $@

majority: majority.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
