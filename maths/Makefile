DBG     = -O
CFLAGS  = -c
DEFINES = -DDEBUG

OBJS   = fsqrt.o \
         prime.o \
         factors.o \
         gcd.o \
         lcm.o \
         driver.o

EXES   = maths

all: ${EXES}

maths: ${OBJS}
	g++ ${DBG} $^ -o $@

%.o: %.cpp
	g++ ${CFLAGS} ${DBG} ${DEFINES} $^ -o $@

clean:
	/bin/rm -rf ${OBJS} ${EXES} core.*
