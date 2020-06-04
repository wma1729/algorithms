DBG     = -g
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = lis.o knapsack.o binpack.o palindrome.o

EXES   = lis knapsack binpack palindrome

all: ${EXES}

lis: lis.o
	g++ ${DBG} $^ -o $@

knapsack: knapsack.o
	g++ ${DBG} $^ -o $@

binpack: binpack.o
	g++ ${DBG} $^ -o $@

palindrome: palindrome.o
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
