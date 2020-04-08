DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = bst.o \
         linkedlist.o \
         pq.o

EXES   = bst \
         linkedlist \
         pq

all: ${EXES}

bst: bst.o
	g++ ${DBG} $^ -o $@

linkedlist: linkedlist.o
	g++ ${DBG} $^ -o $@

pq: pq.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
