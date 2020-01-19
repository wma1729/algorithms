DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = nqueen.o \
         sudoku.o \
         subset.o \
         permutations.o \
         wordbreak.o

EXES   = nqueen \
         sudoku \
         subset \
         permutations \
         wordbreak

all: ${EXES}

nqueen: nqueen.o
	g++ ${DBG} $^ -o $@

sudoku: sudoku.o
	g++ ${DBG} $^ -o $@

subset: subset.o
	g++ ${DBG} $^ -o $@

permutations: permutations.o
	g++ ${DBG} $^ -o $@

wordbreak: wordbreak.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
