# GameOfLife-Simple
A very simple implementation of Conway's Game of Life in C. 

To run the code after compilation with Makefile:
./GameOfLife [size of world] [#generations] [probability of living cell] [amount of threads]

e.g. if the following is wanted: 1000 x 1000 grid, 500 generations, 30 % probability of the cell initially being
alive, and 5 threads running: ./GameOfLife 1000 500 3 5.



Optimized for CPU Intel Core i5-8250U CPU @ 1.60GHz, using compiler version gcc (Ubuntu 9.3.0-17ubuntu1 20.04) 9.3.0
