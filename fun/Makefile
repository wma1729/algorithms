DBG     = -g
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = towersofhanoi.o skyline.o

EXES   = towersofhanoi skyline

all: ${EXES}

towersofhanoi: towersofhanoi.o
	g++ ${DBG} $^ -o $@

skyline: skyline.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
