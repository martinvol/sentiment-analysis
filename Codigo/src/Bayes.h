#ifndef BAYES_H_
#define BAYES_H_

#include <vector>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

class Bayes {
private:


public:

	Bayes();

	virtual ~Bayes();

	std::vector<std::string> ids;

	std::unordered_map< std::string, std::vector<int>* >* data;

	std::unordered_map< std::string, std::unordered_map< std::string, std::vector<int>* > > data_orden2;
	
	// entrenar, recibimos oraciones. Oraciones entiendo como un string que representa una oracion
	// y si es positivo o negativo
	
	//void entrenar(int tag, std::vector<std::string*>* oracion);
	long double calcular_probas(std::string word, std::vector<int>*);
	void entrenar(int tag, std::string word);
	void entrenar_orden2(int tag, std::string word, std::string previus);

	void aprender (int tag, boost::sregex_token_iterator oraciones);
	long double evaluar (boost::sregex_token_iterator oraciones);
	std::vector<long double>* Predicciones();

};

#endif /* BAYES_H_ */
