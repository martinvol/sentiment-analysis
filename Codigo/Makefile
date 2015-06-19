CFLAGS=-g -Wall -std=c++11 -pedantic -O3
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes
BOSTFLAGS =  -lboost_system -lboost_regex



main.o: src/SA.cpp
	$(CC) "src/SA.cpp" -c $(CFLAGS)

main_bayes.o: src/bayes_main.cpp
	$(CC) "src/bayes_main.cpp" -c $(CFLAGS) $(BOSTFLAGS)

Perceptron.o: src/Perceptron.cpp src/Perceptron.h
	$(CC) "src/Perceptron.cpp" -c $(CFLAGS)

Bayes.o: src/Bayes.cpp src/Bayes.h
	$(CC) "src/Bayes.cpp" -c $(CFLAGS) $(BOSTFLAGS)


compilar: Perceptron.o main.o
	$(CC) *.o $(CFLAGS)  -lm -o ejecutable $(BOSTFLAGS)


compilar_bayes: main_bayes.o Bayes.o
	$(CC) *.o $(CFLAGS)  -lm -o ejecutable $(BOSTFLAGS)


clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "a.out" -type f -delete
	find . -name "main" -type f -delete