CFLAGS=-g -Wall -std=c++11 -pedantic -O3 
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes
BOSTFLAGS =  -lboost_system -lboost_regex



main.o: src/SA.cpp
	$(CC) "src/SA.cpp" -c


Perceptron.o: src/Perceptron.cpp src/Perceptron.h
	$(CC) "src/Perceptron.cpp" -c

Bayes.o: src/Bayes.cpp src/Bayes.h
	$(CC) "src/Bayes.cpp" -c


compilar: Perceptron.o main.o Bayes.o
	$(CC) *.o $(CFLAGS)  -lm -o ejecutable $(BOSTFLAGS)
