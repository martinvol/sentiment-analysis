CFLAGS=-g -Wall -std=c++11 -pedantic -O3
CC=g++
VG=valgrind
VFLAGS=--leak-check=full --show-reachable=yes
BOSTFLAGS =  -lboost_system -lboost_regex

tp: compilar compilar_bayes combiner
	
	./ejecutableB -u # Bayes usa los unlabeled
	rm -f outBayes.csv
	touch outBayes.csv
	cat labeledTrainData.tsv >> outBayes.csv
	cat outBayes.csvt >> outBayes.csv
	rm outBayes.csvt
	./ejecutable -b # le pasamos esa salida a bayes + los 25K de kaggle

	./ejecutable # tron solo con train Data

	./ejecutableB # ejecutable solo con la data de Kaggle

	# touch temp
	# echo '"id","sentiment"' > temp
	# cat OnlyBayes.csv >> temp
	# mv temp OnlyBayes.csv



	./boosting 

combiner:
	$(CC) "src/combiner.cpp" -o boosting $(CFLAGS)

main.o: src/SA.cpp Perceptron.o
	$(CC) "src/SA.cpp" -c $(CFLAGS)

main_bayes.o: src/bayes_main.cpp Bayes.o
	$(CC) "src/bayes_main.cpp" -c $(CFLAGS) $(BOSTFLAGS)

Perceptron.o: src/Perceptron.cpp src/Perceptron.h
	$(CC) "src/Perceptron.cpp" -c $(CFLAGS)

Bayes.o: src/Bayes.cpp src/Bayes.h
	$(CC) "src/Bayes.cpp" -c $(CFLAGS) $(BOSTFLAGS)


compilar: Perceptron.o main.o
	$(CC) Perceptron.o SA.o $(CFLAGS)  -lm -o ejecutable $(BOSTFLAGS)


compilar_bayes: main_bayes.o Bayes.o
	$(CC) bayes_main.o Bayes.o $(CFLAGS)  -lm -o ejecutableB $(BOSTFLAGS)


clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "a.out" -type f -delete
	find . -name "main" -type f -delete
	rm -f boosting
	rm -f ejecutable
	rm -f ejecutableB
