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
	std::vector<double> pesos;
	std::vector<std::string> ids;
	int dimensiones;
	float rate;
	int pasadas;
	int tolerancia;


public:
	Perceptron(int dimension, float learning_rate, int numero_pasadas, int nro_errores);
	virtual ~Perceptron();

	void Entrenar();
	// std::vector< std::vector<double> > Predicciones();
	double Sigmoid(double activacion);
	std::vector<double> Predicciones();
	void Agregar(int tag,int *errores,std::vector<unsigned long>& features);
	double Clasificar(std::vector<unsigned long>& features);
	std::vector<std::string> ObtenerIds();
};

#endif /* PERCEPTRON_H_ */
