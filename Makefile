DBG     = -O
CFLAGS  = -c
DEFINES =

OBJS   = nqueen.o \
         subset.o \
         permutations.o \
         towersofhanoi.o \
         graph.o \
         bst.o \
         linkedlist.o \
         wordbreak.o

EXES   = nqueen \
         subset \
         permutations \
         towersofhanoi \
         graph \
         bst \
         linkedlist \
         wordbreak

all: ${EXES}

nqueen: nqueen.o
	g++ ${DBG} $^ -o $@

subset: subset.o
	g++ ${DBG} $^ -o $@

permutations: permutations.o
	g++ ${DBG} $^ -o $@

towersofhanoi: towersofhanoi.o
	g++ ${DBG} $^ -o $@

graph: graph.o
	g++ ${DBG} $^ -o $@

bst: bst.o
	g++ ${DBG} $^ -o $@

linkedlist: linkedlist.o
	g++ ${DBG} $^ -o $@

wordbreak: wordbreak.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES}
