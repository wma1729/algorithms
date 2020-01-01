
DBG    = -O
CFLAGS = -c

OBJS   = nqueen.o \
         subset.o \
         permutations.o \
         graph.o \
         bst.o \
         linkedlist.o

EXES   = nqueen \
         subset \
         permutations \
         graph \
         bst \
         linkedlist

all: ${EXES}

nqueen: nqueen.o
	g++ ${DBG} $^ -o $@

subset: subset.o
	g++ ${DBG} $^ -o $@

permutations: permutations.o
	g++ ${DBG} $^ -o $@

graph: graph.o
	g++ ${DBG} $^ -o $@

bst: bst.o
	g++ ${DBG} $^ -o $@

linkedlist: linkedlist.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES}
