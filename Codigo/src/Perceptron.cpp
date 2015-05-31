/*
 * Perceptron.cpp
 *
 *  Created on: May 27, 2015
 *      Author: seba
 */

#include "Perceptron.h"

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
		int errores = 0;
		// TODO parsear texto y devolver 25000 vectores con N features y el tag al final
		// TODO for each vector de features y su tag
			std::vector<unsigned long> features; //TODO para que compile
			int tag = 0;
			Perceptron::Agregar(tag,&errores,features);

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

double Perceptron::Clasificar(std::vector<unsigned long>& features) {
	double activacion;

	for (std::vector<unsigned long>::iterator it = features.begin(); it !=features.end(); ++it){
		activacion += pesos[*it];
	}

	return activacion;
}

void Perceptron::Agregar(int tag, int& errores, std::vector<unsigned long>& features) {
	double activacion;

	for (std::vector<unsigned long>::iterator it = features.begin(); it !=features.end(); ++it){
		activacion += pesos[*it];
	}
	if (activacion * tag <= 0){ // Esto significa que clasifico erroneamente
		errores++;
		// updateo los pesos utilizados
		for (std::vector<unsigned long>::iterator iter = features.begin(); iter !=features.end(); ++iter){
			pesos[*iter] += (tag);
		}
	}
}
