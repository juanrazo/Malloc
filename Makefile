CFLAGS	= -g
CC	= gcc
OBJ	= myAllocatorTest1 test1 testResize testNextFit

all: $(OBJ)

myAllocatorTest1: myAllocator.o myAllocatorTest1.o
	$(CC) $(CFLAGS) -o $@ $^

test1: myAllocator.o malloc.o test1.o
	$(CC) $(CFLAGS) -o $@ $^

testResize: myAllocator.o testResize.o
	$(CC) $(CFLAGS) -o $@ $^

testNextFit: myAllocator.o testNextFit.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(OBJ) 

