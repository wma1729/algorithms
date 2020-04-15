DBG     = -g -Wall
CFLAGS  = -c
DEFINES =

OBJS   = search.o

EXES   = search

all: ${EXES}

search: search.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
