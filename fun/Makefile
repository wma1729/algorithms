DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = towersofhanoi.o

EXES   = towersofhanoi

all: ${EXES}

towersofhanoi: towersofhanoi.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
