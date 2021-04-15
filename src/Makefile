CC = gcc
CFLAGS = -O3 -Wall

Project2: main.o
	gcc main.o -o main
main.o: main.c
	gcc main.c -c
clean:
	rm -rf main.o main

encrypt: main.c
	gcc -pthread -o encrypt main.c

clean:
	rm encrypt