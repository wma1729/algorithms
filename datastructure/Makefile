DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = bst.o \
         linkedlist.o \
         heap.o

EXES   = bst \
         linkedlist \
         heap

all: ${EXES}

bst: bst.o
	g++ ${DBG} $^ -o $@

linkedlist: linkedlist.o
	g++ ${DBG} $^ -o $@

heap: heap.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
