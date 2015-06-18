
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

// mejor resultado, trigramas + bigramas + boosting

#define SINGLES false
#define BIGRAMS true
#define TRIGRAMS false
#define JUMPER false
#define BOOSTING true
#define LABELED "labeledTrainData.tsv"
#define UNLABELED "testData.tsv"

/*#define LABELED "Data/labeledTrainDataStemmedAndLemmalitized.tsv"
#define UNLABELED "testDataStemmedAndLemmalitized.tsv"*/

/*
bool check_positive_negative(review, data){
    total = 0
    for w in review.split():
        proba =  float((*data)[w][0])/(float((*data)[w][0]) + float((*data)[w][1]))
        total += proba
    return 1 if total/float(len(review.split())) >= 0.5 else 0
}*/

typedef std::vector<std::vector<std::string> > Rows;


Bayes::Bayes(){
	char const row_delim = '\n';
	char const field_delim = '\t';
	std::string firstrow;
	data = new std::unordered_map< std::string, std::vector<int>* >();

	Rows rows;
	std::ifstream input(LABELED);
	if (!input) {
		std::cout << "unable to load file" << std::endl;
	}
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
		aprender(tag, iter);
	}

	if (BOOSTING){
		fprintf(stderr,"Empieza el boosting\n");

		bool termine = false;
		int pasadas = 0;
		while (!termine){
			termine = true;
			std::ifstream input(LABELED);
			if (!input) {
				std::cout << "unable to load file" << std::endl;
			}

			int errores = 0;
			for (std::vector<std::vector<std::string> >::iterator it = rows.begin(); it !=rows.end(); ++it){
				int tag = std::stoi((*it)[1]);
				std::string text = (*it)[2];

				boost::regex regex("\\w+");
				boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
				float first_try = evaluar(iter);
				if ((first_try > 0.5) != tag && (first_try != 0.5)){
					errores++;
					while ((evaluar(iter) > 0.5) != tag){
						//printf("evaluar %Le\n", evaluar(iter) );
						termine = false;
						boost::regex regex("\\w+");
						boost::sregex_token_iterator iter2(text.begin(), text.end(), regex, 0);
						aprender(tag, iter2);
					}
				}

			}
			fprintf(stderr,"nro de pasada: %d, cantidad de errores %d \n",pasadas,errores);
			pasadas++;
		}
	}
}

Bayes::~Bayes(){
	
}

long double Bayes::calcular_probas(std::string word, std::vector<int>* frec_vectors){
	long double neg = (*frec_vectors)[0];
	long double poss = (*frec_vectors)[1];
	return poss/(neg + poss);
}

// void Bayes::entrenar(int tag, std::vector<std::string*>* oracion){
void Bayes::entrenar(int tag, std::string word){
	std::string* palabra;
		
	palabra = &word;
	//std::cout << palabra << "\n";
	

	if(data->find(*palabra) == data->end()){
		(*(data))[*palabra] = new std::vector<int>(2);
		(*(*data)[*palabra])[0] = 0;
		(*(*data)[*palabra])[1] = 0;
		
	}
	
	if(tag == 0){
		(*(*data)[*palabra])[0]++;
	}else{
		(*(*data)[*palabra])[1]++;
	}

}

void Bayes::entrenar_orden2(int tag, std::string word, std::string previus){
	if (data_orden2.find(previus) == data_orden2.end()){
		// no está la palabra anterior
		//data_orden2[previus] = new std::unordered_map< std::string, std::vector<int>*>();


	}

}

void Bayes::aprender (int tag, boost::sregex_token_iterator iter){
	boost::sregex_token_iterator end;

	// Tengo que hacer la iteracion una vez antes del for:
	std::string pal_ant_ant(*iter);
	std::transform(pal_ant_ant.begin(), pal_ant_ant.end(), pal_ant_ant.begin(), ::tolower);
	// palabra está en: pal_ant
	
	if (SINGLES)
		entrenar(tag, pal_ant_ant);
	iter++;

	std::string pal_ant(*iter);
	std::transform(pal_ant.begin(), pal_ant.end(), pal_ant.begin(), ::tolower);
	// palabra está en: pal_ant
	
	if (SINGLES)
		entrenar(tag, pal_ant_ant);
	iter++;

	if (BIGRAMS){
		std::string gram(pal_ant_ant + " " + pal_ant);
		entrenar(tag, gram);
	}

	for( ; iter != end; ++iter ) {

		std::string temp(*iter);
		std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		if (SINGLES)
			entrenar(tag, temp);
		//hash_palabras.push_back(str_hash % dimensiones);

		if (BIGRAMS){
			std::string gram(pal_ant + " " + temp);
			// printf(" %s", gram.c_str());
			std::transform(gram.begin(), gram.end(), gram.begin(), ::tolower);
			entrenar(tag, gram);
			pal_ant = temp;
		}
		if (TRIGRAMS){
			std::string tri(pal_ant_ant + " " + pal_ant + " " + temp);
			std::transform(tri.begin(),tri.end(),tri.begin(), ::tolower);
			entrenar(tag, tri);
		}
		if (JUMPER){
			std::string tri(pal_ant_ant + " " + temp);
			std::transform(tri.begin(),tri.end(),tri.begin(), ::tolower);
			entrenar(tag, tri);	
		}

		pal_ant = temp;
		pal_ant_ant = pal_ant;
	}
}


long double Bayes::evaluar (boost::sregex_token_iterator iter){
			// Tengo que hacer la iteracion una vez antes del for:
	boost::sregex_token_iterator end;
	long double suma_probas = 0;
	float cantidad_palabras = 0; // es una cantidad, pero no quiero castear después
	
	std::string pal_ant_ant(*iter);
	std::transform(pal_ant_ant.begin(), pal_ant_ant.end(), pal_ant_ant.begin(), ::tolower);
	// palabra está en: pal_ant
	
	if (SINGLES){
		auto saved = data->find(pal_ant_ant);
			// la clave existe
		if (saved != data->end()){
			long double proba =calcular_probas(pal_ant_ant, (*data)[pal_ant_ant]);// ((long double)(*(*data)[pal_ant])[1])/(((long double)(*(*data)[pal_ant])[0]) + ((long double)(*(*data)[pal_ant])[1]));
			suma_probas += proba;
			cantidad_palabras++;
		}
	}
	iter++;


	std::string pal_ant(*iter);

	std::transform(pal_ant.begin(), pal_ant.end(), pal_ant.begin(), ::tolower);
	// palabra está en: pal_ant
	

	if (SINGLES){
		auto saved = data->find(pal_ant);
			// la clave existe
		if (saved != data->end()){
			long double proba =calcular_probas(pal_ant, (*data)[pal_ant]);// ((long double)(*(*data)[pal_ant])[1])/(((long double)(*(*data)[pal_ant])[0]) + ((long double)(*(*data)[pal_ant])[1]));
			suma_probas += proba;
			cantidad_palabras++;
		}
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

		//long double proba = ((long double) *(*data)[temp] )[1]/( (long double) *(*data)[temp])[0] + (long double) *(*data)[temp])[1]);
		auto saved2 = data->find(temp);
		// la clave existe
		if (saved2 != data->end()){
			long double proba = calcular_probas(temp ,(*data)[temp]);//((long double)(*(*data)[temp])[1])/(((long double)(*(*data)[temp])[0]) + ((long double)(*(*data)[temp])[1]));
			suma_probas += proba;
			cantidad_palabras++;
		}
		if (BIGRAMS){
			std::string gram(pal_ant + " " + temp);
			// printf(" %s", gram.c_str());
			std::transform(gram.begin(), gram.end(), gram.begin(), ::tolower);
			auto saved3 = data->find(gram);
			// la clave existe
			if (saved3 != data->end()){
				long double proba = calcular_probas(gram,(*data)[gram]);// ((long double)(*(*data)[gram])[1])/(((long double)(*(*data)[gram])[0]) + ((long double)(*(*data)[gram])[1]));
				suma_probas += proba;
				cantidad_palabras++;
			}
		}
		if (TRIGRAMS){
			std::string tri(pal_ant_ant + " " + pal_ant + " " + temp);
			std::transform(tri.begin(),tri.end(),tri.begin(), ::tolower);
			auto saved3 = data->find(tri);
			// la clave existe
			if (saved3 != data->end()){
				long double proba = calcular_probas(tri,(*data)[tri]);// ((long double)(*(*data)[gram])[1])/(((long double)(*(*data)[gram])[0]) + ((long double)(*(*data)[gram])[1]));
				suma_probas += proba;
				cantidad_palabras++;
			}
		}
		if (JUMPER){
			std::string tri(pal_ant_ant + " " + temp);
			std::transform(tri.begin(),tri.end(),tri.begin(), ::tolower);
			auto saved3 = data->find(tri);
			// la clave existe
			if (saved3 != data->end()){
				long double proba = calcular_probas(tri,(*data)[tri]);// ((long double)(*(*data)[gram])[1])/(((long double)(*(*data)[gram])[0]) + ((long double)(*(*data)[gram])[1]));
				suma_probas += proba;
				cantidad_palabras++;
			}
		}
		pal_ant = temp;
		pal_ant_ant = pal_ant;
	}
	if (cantidad_palabras != 0){
		return suma_probas/cantidad_palabras;
		//pred->push_back();
	} else {
		puts("Un error");
		return 0.5;
		//pred->push_back(0.5); // no se que está pasando acá
	}
	return 0;
}

std::vector<long double>* Bayes::Predicciones(){
	Rows rows;
	long double max=0, min=1;
	std::ifstream input(UNLABELED);
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
		

		long double proba = evaluar(iter);
		if (proba > max) max = proba;
		if (proba < min) min = proba;
		pred->push_back(proba);
		// predicciones.push_back(pred);


		//pred->push_back();

	}
	for (std::vector<long double >::iterator it = pred->begin(); it !=pred->end(); ++it){
		(*it) = (((*it) - min) / (max - min));
	}
	input.close();
	return pred;
}

