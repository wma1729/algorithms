DBG     = -g -Wall
CFLAGS  = -c
DEFINES =

OBJS   = sort.o dutchflag.o genrand.o blackwhite.o shuffle.o

EXES   = sort dutchflag genrand blackwhite shuffle

all: ${EXES}

sort: sort.o
	g++ ${DBG} $^ -o $@

dutchflag: dutchflag.o
	g++ ${DBG} $^ -o $@

genrand: genrand.o
	g++ ${DBG} $^ -o $@

blackwhite: blackwhite.o
	g++ ${DBG} $^ -o $@

shuffle: shuffle.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
