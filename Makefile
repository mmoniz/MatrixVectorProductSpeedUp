
CC      = mpicc
CFLAGS  = -Wall -ansi -O2

OBJECTS = genmatvec.o matvecres.o dotproduct.o common.o

all: A2perf A2perf2

A2perf : $(OBJECTS) as2q1.o
	$(CC) -o $@  $(OBJECTS) as2q1.o

A2perf2 : $(OBJECTS) as2q2.o
	$(CC) -o $@  $(OBJECTS) as2q2.o

clean:
	rm *.o A2perf*

