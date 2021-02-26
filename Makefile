CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS = -lwiringPi

all:	lab2a lab2b

lab2a:		lab2a.o
	$(CC) $(LFLAGS) -o lab2a lab2a.o

lab2a.o:	lab2a.c
	$(CC) $(CFLAGS) -c lab2a.c

lab2b:		lab2b.o
	$(CC) $(LFLAGS) -o lab2b lab2b.o

lab2b.o:	lab2b.c
	$(CC) $(CFLAGS) -c lab2b.c

clean:
	rm -f *~ *.o lab2a lab2b
