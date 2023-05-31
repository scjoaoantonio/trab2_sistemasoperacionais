CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

OBJ = main.o
HDR = simulador.h

all: tp2virtual

tp2virtual: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o tp2virtual

main.o: main.c $(HDR)
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(OBJ) tp2virtual