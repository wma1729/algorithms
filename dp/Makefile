DBG     = -g
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = knapsack.o

EXES   = knapsack

all: ${EXES}

knapsack: knapsack.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
