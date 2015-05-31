/*
 * Perceptron.cpp
 *
 *  Created on: May 27, 2015
 *      Author: seba
 */

#include "Perceptron.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <tr1/functional>

Perceptron::Perceptron(int dimension, int learning_rate, int numero_pasadas, int nro_errores) {
	pesos.resize(dimension);
	std::fill(pesos.begin(), pesos.end(), 0);
	// pesos.shrink_to_fit();
	dimensiones = dimension;
	rate = learning_rate;
	pasadas = numero_pasadas;
	tolerancia = nro_errores;

}

Perceptron::~Perceptron() {

}

void Perceptron::Entrenar(){

	//Iteraciones de entrenamiento
	for (int i = 0; i <= pasadas; i++ ){
		fprintf(stderr,"hola1 \n");
		int errores = 0;
		typedef std::vector<std::vector<std::string> > Rows;
		Rows rows;
		std::ifstream input ("./labeledTrainData.tsv");
		if (!input) {
			std::cout << "unable to load file" << std::endl;
		}
		char const row_delim = '\n';
		char const field_delim = '\t';
		for (std::string row; getline(input, row, row_delim); ) {
			fprintf(stderr,"hola2 \n");
			rows.push_back(Rows::value_type());
			std::istringstream ss(row);
			for (std::string field; getline(ss, field, field_delim); ) {
				rows.back().push_back(field);
			}
		}
		for (std::vector<std::vector<std::string> >::iterator it = rows.begin(); it !=rows.end(); ++it){
			fprintf(stderr,"hola3 \n");
			int tag = std::stoi((*it)[0]);
			std::string text = (*it)[2];
		
			boost::regex regex("\\w+");
			boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
			boost::sregex_token_iterator end;
			std::vector<std::size_t> hash_palabras; 

			for( ; iter != end; ++iter ) {
				printf("hola3 \n");
				std::tr1::hash<std::string> hash_fn;
				std::size_t str_hash = hash_fn(*iter);
				hash_palabras.push_back(str_hash);
				std::cout << str_hash << std::endl;
			}

			Perceptron::Agregar(tag,&errores,hash_palabras);
		}

		if (errores <= tolerancia){
			//hay overfit, frenamos
			break;
		}
	}
}

std::vector< std::vector<double> > Perceptron::Predicciones(){
	std::vector< std::vector<double> > predicciones;
	// for each feature vector
		std::vector<unsigned long> feat_vect; // TODO, para que compile
		std::vector<double> pred;
		pred.push_back(Perceptron::Clasificar(feat_vect));
		predicciones.push_back(pred);

	return predicciones;
}

double Perceptron::Clasificar(std::vector<unsigned long> &features) {
	double activacion = 0; // AAAAAAAAAAAAAAAAAAAAAAAa

	for (std::vector<unsigned long>::iterator it = features.begin(); it !=features.end(); ++it){
		activacion += pesos[*it];
	}

	return activacion;
}

void Perceptron::Agregar(int tag, int* errores, std::vector<unsigned long>& features) {
	double activacion = 0; //AAAAAAAAAAAAAAAAAAAAAAAAAa

	for (std::vector<unsigned long>::iterator it = features.begin(); it !=features.end(); ++it){
		activacion += pesos[*it];
	}
	if (activacion * tag <= 0){ // Esto significa que clasifico erroneamente
		(*errores)++;
		// updateo los pesos utilizados
		for (std::vector<unsigned long>::iterator iter = features.begin(); iter !=features.end(); ++iter){
			pesos[*iter] += (tag);
		}
	}
}
