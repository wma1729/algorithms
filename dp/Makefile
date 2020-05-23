DBG     = -g
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = lis.o knapsack.o

EXES   = lis knapsack

all: ${EXES}

lis: lis.o
	g++ ${DBG} $^ -o $@

knapsack: knapsack.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
