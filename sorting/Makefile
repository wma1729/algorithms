DBG     = -O -Wall
CFLAGS  = -c
DEFINES = 

OBJS   = sort.o

EXES   = sort

all: ${EXES}

sort: sort.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
