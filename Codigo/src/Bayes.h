#ifndef BAYES_H_
#define BAYES_H_

#include <vector>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <string>
#include <stdlib.h>

class Bayes {
private:


public:

	Bayes();

	virtual ~Bayes();

	std::unordered_map< std::string, std::vector<int>* > data;
	
	// entrenar, recibimos oraciones. Oraciones entiendo como un string que representa una oracion
	// y si es positivo o negativo
	
	void entrenar(int tag, std::vector<std::string*>* oracion);

	double evaluar (std::vector<std::vector<std::string*>*>* oraciones);

};

#endif /* BAYES_H_ */
