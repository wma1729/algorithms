DBG     = -g
CFLAGS  = -c
DEFINES =

OBJS   = graph.o wgraph.o

EXES   = graph wgraph

all: ${EXES}

graph: graph.o
	g++ ${DBG} $^ -o $@

wgraph: wgraph.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
