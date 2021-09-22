

CC = gcc
LD = gccs
CFLAGS = -Ofast -march=native -fopenmp -Wall -g 
LDFLAGS = -lm
EXECUTABLE = GameOfLife

GameOfLife: GameOfLife.c
	$(CC) $(CFLAGS) -o $(EXECUTABLE) GameOfLife.c $(LDFLAGS)

clean: 
	$(RM) ./GameOfLife