DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = towersofhanoi.o \
         graph.o \
         bst.o \
         linkedlist.o

EXES   = towersofhanoi \
         graph \
         bst \
         linkedlist

all: ${EXES}

towersofhanoi: towersofhanoi.o
	g++ ${DBG} $^ -o $@

graph: graph.o
	g++ ${DBG} $^ -o $@

bst: bst.o
	g++ ${DBG} $^ -o $@

linkedlist: linkedlist.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
