all: nqueen subset permutations graph

DBG    = -O
CFLAGS = -c
OBJS   = nqueen.o \
         subset.o \
         permutations.o \
         graph.o

nqueen: nqueen.o
	g++ ${DBG} $^ -o $@

subset: subset.o
	g++ ${DBG} $^ -o $@

permutations: permutations.o
	g++ ${DBG} $^ -o $@

graph: graph.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} nqueen subset permutations graph
