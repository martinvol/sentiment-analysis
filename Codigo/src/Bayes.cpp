
#include "Bayes.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <tr1/functional>
#include <locale>         // std::locale, std::tolower
#include <math.h>
#include <cmath> 

#define BIGRAMS true

/*
bool check_positive_negative(review, data){
    total = 0
    for w in review.split():
        proba =  float(data[w][0])/(float(data[w][0]) + float(data[w][1]))
        total += proba
    return 1 if total/float(len(review.split())) >= 0.5 else 0
}*/

typedef std::vector<std::vector<std::string> > Rows;


Bayes::Bayes(){

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

			// Tengo que hacer la iteracion una vez antes del for:
			std::string pal_ant(*iter);
			std::transform(pal_ant.begin(), pal_ant.end(), pal_ant.begin(), ::tolower);
			// palabra está en: pal_ant
			
			entrenar(tag, pal_ant);
			iter++;

			for( ; iter != end; ++iter ) {
				//std::tr1::hash<std::string> hash_fn;
				/*std::transform((*iter).begin(), (*iter).end(), (*iter).begin(), ::tolower);*/
				std::string temp(*iter);
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				entrenar(tag, temp);
				//hash_palabras.push_back(str_hash % dimensiones);

				if (BIGRAMS){
					std::string gram(pal_ant + " " + temp);
					// printf(" %s", gram.c_str());
					std::transform(gram.begin(), gram.end(), gram.begin(), ::tolower);
					entrenar(tag, gram);
					pal_ant = temp;
				}
			}
		}

}

Bayes::~Bayes(){
	
}

// void Bayes::entrenar(int tag, std::vector<std::string*>* oracion){
void Bayes::entrenar(int tag, std::string word){
	std::string* palabra;
		
	palabra = &word;
	//std::cout << palabra << "\n";
	

	if(data.find(*palabra) == data.end()){
		data[*palabra] = new std::vector<int>(2);
		(*data[*palabra])[0] = 0;
		(*data[*palabra])[1] = 0;
		
	}
	
	if(tag == 0){
		(*data[*palabra])[0]++;
	}else{
		(*data[*palabra])[1]++;
	}

}

void Bayes::entrenar_orden2(int tag, std::string word, std::string previus){
	if (data_orden2.find(previus) == data_orden2.end()){
		// no está la palabra anterior
		//data_orden2[previus] = new std::unordered_map< std::string, std::vector<int>*>();


	}

}

double evaluar (std::vector<std::vector<std::string*>*>* oraciones);

std::vector<long double>* Bayes::Predicciones(){
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
	std::vector<long double>* pred = new std::vector<long double>();
	for (std::vector<std::vector<std::string> >::iterator it = rows.begin(); it !=rows.end(); ++it){
		std::string id = (*it)[0];
		ids.push_back(id);

		std::string text = (*it)[1];
	
		boost::regex regex("\\w+");
		boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
		boost::sregex_token_iterator end;

		// Tengo que hacer la iteracion una vez antes del for:
		std::string pal_ant(*iter);
		std::transform(pal_ant.begin(), pal_ant.end(), pal_ant.begin(), ::tolower);
		// palabra está en: pal_ant
		
		long double suma_probas = 0;
		float cantidad_palabras = 0; // es una cantidad, pero no quiero castear después

		auto saved = data.find(pal_ant);
			// la clave existe
		if (saved != data.end()){
			long double proba = ((long double)(*data[pal_ant])[1])/(((long double)(*data[pal_ant])[0]) + ((long double)(*data[pal_ant])[1]));
			suma_probas += proba;
			cantidad_palabras++;
		}
		iter++;

		// Tengo que hacer la iteracion una vez antes del for:
		//std::string pal_ant(*iter);
		//std::transform(pal_ant.begin(), pal_ant.end(), pal_ant.begin(), ::tolower);
		// palabra está en: pal_ant
		
		for( ; iter != end; ++iter ) {
			//std::tr1::hash<std::string> hash_fn;
			/*std::transform((*iter).begin(), (*iter).end(), (*iter).begin(), ::tolower);*/
			std::string temp(*iter);
			std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

			//long double proba = ((long double) *data[temp] )[1]/( (long double) *data[temp])[0] + (long double) *data[temp])[1]);
			auto saved2 = data.find(temp);
			// la clave existe
			if (saved2 != data.end()){
				long double proba = ((long double)(*data[temp])[1])/(((long double)(*data[temp])[0]) + ((long double)(*data[temp])[1]));
				suma_probas += proba;
				cantidad_palabras++;
			}
			if (BIGRAMS){
					std::string gram(pal_ant + " " + temp);
					// printf(" %s", gram.c_str());
					std::transform(gram.begin(), gram.end(), gram.begin(), ::tolower);
					auto saved3 = data.find(gram);
					// la clave existe
					if (saved3 != data.end()){
						long double proba = ((long double)(*data[gram])[1])/(((long double)(*data[gram])[0]) + ((long double)(*data[gram])[1]));
						suma_probas += proba;
						cantidad_palabras++;
					}
					// entrenar(tag, pal_ant);
					pal_ant = temp;
				}
			
			//hash_palabras.push_back(str_hash % dimensiones);
			/*if (bigramas){
				std::string gram(pal_ant + " " + temp);
				// printf("Bigrama %s", gram.c_str());
				std::transform(gram.begin(), gram.end(), gram.begin(), ::tolower);
				std::size_t gram_hash = hash_fn(gram);
				hash_palabras.push_back(gram_hash % dimensiones);
				pal_ant = temp;
			}*/
		}
		// pred->push_back(suma_probas/cantidad_palabras);
		if (cantidad_palabras != 0){
			pred->push_back(suma_probas/cantidad_palabras);
		} else {
			puts("Un error");
			pred->push_back(0.5); // no se que está pasando acá
		}
	}
	input.close();
	return pred;
}

