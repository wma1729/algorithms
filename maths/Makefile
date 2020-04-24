DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = fsqrt.o \
         prime.o \
         factors.o \
         gcd.o \
         lcm.o \
         power.o \
         driver.o

EXES   = maths polynomial

all: ${EXES}

maths: ${OBJS}
	g++ ${DBG} $^ -o $@

polynomial: polynomial.cpp
	g++ ${DBG} ${DEFINES} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
