//============================================================================
// Name        : SA.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <tr1/functional>
#include "Bayes.h"
#include <cmath>
#include <iomanip>
#include <limits>

int main() {
/*
	std::cout.precision(190);


	int dimension = pow(2,28.5);
	fprintf(stderr,"%d",dimension);
	float rate = 0.1;
	int pasadas = 80;
	int errores = 0;
	bool bigramas = true;
	Perceptron miPerceptron(dimension, rate, pasadas, errores, bigramas);
	miPerceptron.Entrenar();
	
	std::vector<long double> preds = miPerceptron.Predicciones();
	std::ofstream myfile ("submission.csv", std::ofstream::trunc);
	if (!myfile) {
		fprintf(stderr,"error al abrir \n");
	}

	myfile << "\"id\",\"sentiment\"";
	myfile << "\n";
	std::vector<std::string> ids = miPerceptron.ObtenerIds();
	for (int i = 0; i < 25000; i++){
		myfile << ids[i];
		myfile << ",";
		myfile << std::fixed << std::setprecision(53) << preds[i];
		myfile << "\n";
		//std::cout << std::fixed << std::cout.precision(190) << preds[i] << std::endl;
	}

	myfile.close();

	Bayes miBayes;*/

	Bayes myBayes;
	puts("Terminé de aprender");
	std::vector<long double>* preds = myBayes.Predicciones();
	puts("Terminé de labelear");

	std::ofstream myfile ("submission.csv", std::ofstream::trunc);
	if (!myfile) {
		fprintf(stderr,"error al abrir \n");
	}

	myfile << "\"id\",\"sentiment\"";
	myfile << "\n";
	std::vector<std::string> ids = myBayes.ids;
		for (int i = 0; i < 25000; i++){
		myfile << ids[i];
		myfile << ",";
		myfile << std::fixed << std::setprecision(53) << (*preds)[i];
		myfile << "\n";
		//std::cout << std::fixed << std::cout.precision(190) << preds[i] << std::endl;
	}

	myfile.close();

	return 0;

}
