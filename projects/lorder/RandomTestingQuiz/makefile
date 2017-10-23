CFLAGS = -Wall -std=c99

all: testme

default: testme

testme.o: testme.c
	gcc -c testme.c -o testme.o

testme: testme.o
	gcc testme.o -o testme

clean:
	-rm -f testme.o
	-rm -f testme