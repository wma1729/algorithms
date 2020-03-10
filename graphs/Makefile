DBG     = -g
CFLAGS  = -c
DEFINES =

OBJS   = graph.o

EXES   = graph

all: ${EXES}

graph: graph.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
