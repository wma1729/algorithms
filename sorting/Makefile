DBG     = -g -Wall
CFLAGS  = -c
DEFINES =

OBJS   = sort.o dutchflag.o

EXES   = sort dutchflag

all: ${EXES}

sort: sort.o
	g++ ${DBG} $^ -o $@

dutchflag: dutchflag.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
