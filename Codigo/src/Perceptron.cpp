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
#include <math.h>
#include <cmath> 

#define BIGRAMS false

typedef std::vector<std::vector<std::string> > Rows;

Perceptron::Perceptron(int dimension, float learning_rate, int numero_pasadas, int nro_errores) {
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
		int errores = 0;
		Rows rows;
		std::ifstream input("labeledTrainData.tsv");
		if (!input) {
			std::cout << "unable to load file" << std::endl;
		}
		char const row_delim = '\n';
		char const field_delim = '\t';
		std::string firstrow;
		getline(input, firstrow, row_delim);
		for (std::string row; getline(input, row, row_delim); ) {
			rows.push_back(Rows::value_type());
			std::istringstream ss(row);
			for (std::string field; getline(ss, field, field_delim); ) {
				rows.back().push_back(field);
			}
		}
		for (std::vector<std::vector<std::string> >::iterator it = rows.begin(); it !=rows.end(); ++it){
			int tag = std::stoi((*it)[1]);
			std::string text = (*it)[2];
		
			boost::regex regex("\\w+");
			boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
			boost::sregex_token_iterator end;
			std::vector<std::size_t> hash_palabras; 

			if (BIGRAMS){
				/*for( ; iter != end; ++iter ) {
					std::tr1::hash<std::string> hash_fn;
					std::size_t str_hash = hash_fn(*iter);
					hash_palabras.push_back(str_hash%dimensiones);
				}*/
			}else {
				for( ; iter != end; ++iter ) {
					std::tr1::hash<std::string> hash_fn;
					std::size_t str_hash = hash_fn(*iter);
					hash_palabras.push_back(str_hash%dimensiones);
				}
			}
			Perceptron::Agregar(tag,&errores,hash_palabras);
		}
		fprintf(stderr,"nro de pasada: %d, cantidad de errores %d \n",i,errores);
		if (errores <= tolerancia){
			//hay overfit, frenamos
			break;
		}
	}
}

std::vector<long double>Perceptron::Predicciones(){
	// std::vector< std::vector<long double> > predicciones;

	Rows rows;
	std::ifstream input("testData.tsv");
	if (!input) {
		std::cout << "unable to load file" << std::endl;
	}
	char const row_delim = '\n';
	char const field_delim = '\t';
	std::string firstrow;
	getline(input, firstrow, row_delim);
	for (std::string row; getline(input, row, row_delim); ) {
		rows.push_back(Rows::value_type());
		std::istringstream ss(row);
		for (std::string field; getline(ss, field, field_delim); ) {
			rows.back().push_back(field);
		}
	}
	long double max = 0;
	long double min = 1;
	std::vector<long double> pred;
	for (std::vector<std::vector<std::string> >::iterator it = rows.begin(); it !=rows.end(); ++it){
		// fprintf(stderr,"hola2 \n");
		// fprintf(stderr,"%s",(*it)[1].c_str());
		// fprintf(stderr,"\n");
		std::string id = (*it)[0];
		ids.push_back(id);

		std::string text = (*it)[1];
	
		boost::regex regex("\\w+");
		boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
		boost::sregex_token_iterator end;
		std::vector<std::size_t> hash_palabras; 

		for( ; iter != end; ++iter ) {
			std::tr1::hash<std::string> hash_fn;
			std::size_t str_hash = hash_fn(*iter);
			hash_palabras.push_back(str_hash%dimensiones);
			// std::cout << str_hash << std::endl;
		}
		long double proba = Perceptron::Clasificar(hash_palabras);
		if (proba > max) max = proba;
		if (proba < min) min = proba;
		long double res = (proba - min) / (max - min);
		pred.push_back(res);
		// predicciones.push_back(pred);
	}	

	return pred; // cambiar luego a predicciones
}

std::vector<std::string> Perceptron::ObtenerIds(){
	return ids;
}

long double Perceptron::Sigmoid(long double activacion){
	long double proba = 1 / (1+exp(-1*activacion));
	return proba;
}

long double Perceptron::Clasificar(std::vector<unsigned long> &features) {
	long double activacion = 0;

	for (std::vector<unsigned long>::iterator it = features.begin(); it !=features.end(); ++it){
		activacion += pesos[*it];
	}
	// Normalizacion online ?
	//long double probabilidad = Perceptron::Sigmoid(activacion);

	return activacion;
}

void Perceptron::Agregar(int tag, int* errores, std::vector<unsigned long>& features) {
	long double activacion = 0;

	for (std::vector<unsigned long>::iterator it = features.begin(); it !=features.end(); ++it){
		activacion += pesos[*it];
	}
	
	bool clas = (activacion > 0.5);
	int error = tag - (int)clas;

	if (error != 0){ // Esto significa que clasifico erroneamente
		(*errores)++;
		// updateo los pesos utilizados
		for (std::vector<unsigned long>::iterator iter = features.begin(); iter !=features.end(); ++iter){
			pesos[*iter] = pesos[*iter] + log10(2.0) * error * rate;
		}
	}
}
