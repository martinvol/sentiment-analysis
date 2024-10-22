/*
 * Perceptron.h
 *
 *  Created on: May 27, 2015
 *      Author: seba
 */

#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_
#include <vector>
#include <string>

class Perceptron {
private:
	std::vector<long double> pesos;
	std::vector<std::string> ids;
	std::vector<std::string> reviews;
	int dimensiones;
	float rate;
	int pasadas;
	int tolerancia;
	bool bigramas;
	bool trigramas;
	bool bayes;


public:
	Perceptron(int dimension, float learning_rate, int numero_pasadas, int nro_errores,bool bigrams,bool trigrams, bool bayes);
	virtual ~Perceptron();

	void Entrenar();
	// std::vector< std::vector<long double> > Predicciones();
	long double Sigmoid(long double activacion);
	std::vector<long double> Predicciones(const std::string path);
	void Agregar(int tag,int *errores,std::vector<unsigned long>& features);
	long double Clasificar(std::vector<unsigned long>& features);
	std::vector<std::string> ObtenerIds();
	std::vector<std::string> ObtenerReviews();
};

#endif /* PERCEPTRON_H_ */
