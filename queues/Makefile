# An admittedly primitive Makefile
# To compile, type "make" or make "all"
# To remove files, type "make clean"

CC = gcc
CFLAGS = -Wall
SFLAGS = -pthread
OBJS = main.o

.SUFFIXES: .c .o 

all: main

main: main.o queue.o
	$(CC) $(CFLAGS) $(SFLAGS) -o main main.o queue.o

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f $(OBJS) main
