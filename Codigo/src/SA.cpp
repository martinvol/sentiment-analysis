//============================================================================
// Name        : SA.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Perceptron.h"
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

int main(int argc, char* argv[]) {


    /* std::string text("With all this stuff going down at the moment with MJ i've started listening to his music, watching the odd documentary here and there, watched The Wiz and watched Moonwalker again. Maybe i just want to get a certain insight into this guy who i thought was really cool in the eighties just to maybe make up my mind whether he is guilty or innocent. Moonwalker is part biography, part feature film which i remember going to see at the cinema when it was originally released. Some of it has subtle messages about MJ's feeling towards the press and also the obvious message of drugs are bad m'kay.<br /><br />Visually impressive but of course this is all about Michael Jackson so unless you remotely like MJ in anyway then you are going to hate this and find it boring. Some may call MJ an egotist for consenting to the making of this movie BUT MJ and most of his fans would say that he made it for the fans which if true is really nice of him.<br /><br />The actual feature film bit when it finally starts is only on for 20 minutes or so excluding the Smooth Criminal sequence and Joe Pesci is convincing as a psychopathic all powerful drug lord. Why he wants MJ dead so bad is beyond me. Because MJ overheard his plans? Nah, Joe Pesci's character ranted that he wanted people to know it is he who is supplying drugs etc so i dunno, maybe he just hates MJ's music.<br /><br />Lots of cool things in this like MJ turning into a car and a robot and the whole Speed Demon sequence. Also, the director must have had the patience of a saint when it came to filming the kiddy Bad sequence as usually directors hate working with one kid let alone a whole bunch of them performing a complex dance scene.<br /><br />Bottom line, this movie is for people who like MJ on one level or another (which i think is most people). If not, then stay away. It does try and give off a wholesome message and ironically MJ's bestest buddy in this movie is a girl! Michael Jackson is truly one of the most talented people ever to grace this planet but is he guilty? Well, with all the attention i've gave this subject....hmmm well i don't know because people can be different behind closed doors, i know this for a fact. He is either an extremely nice but stupid guy or one of the most sickest liars. I hope he is not the latter.");
    boost::regex regex("\\w+");

    boost::sregex_token_iterator iter(text.begin(), text.end(), regex, 0);
    boost::sregex_token_iterator end;

    std::vector<std::size_t> hash_palabras; 


    for( ; iter != end; ++iter ) {
    	std::tr1::hash<std::string> hash_fn;

    	std::size_t str_hash = hash_fn(*iter);

    	hash_palabras.push_back(str_hash);
  
    }

	*/
	
    /*long long double a = 1;
    long long double b = 3;*/
	//std::cout.precision(190);

	//printf("%f\n", a/b);
	//printf("%lu\n", sizeof(a));
	//std::cout << a/b << std::endl;

    bool producir_piping = false;
	int dimension = pow(2,26);
	float rate = 0.1;
	int pasadas = 60;
	int errores = 0;
	bool bigramas = true;
	bool trigramas = true;
	bool bayes;
	char path_salida[100];
	if (argc == 2){
		bayes = true;
		strcpy(path_salida, "tronBayes.csv");
	} else {
		bayes = false;
		strcpy(path_salida, "tron.csv");
	}


	Perceptron miPerceptron(dimension, rate, pasadas, errores, bigramas, trigramas, bayes);
	miPerceptron.Entrenar();

	std::ofstream myfile (path_salida, std::ofstream::trunc);
	printf("Using file %s\n", path_salida);

	if (!myfile) {
		fprintf(stderr,"error al abrir \n");
	}

    std::string row_delim = "\n";
	std::string field_delim = ",";
    if (producir_piping){
    	std::cout << "Produciendo piping \n";
    	row_delim = "\n";
		field_delim = "\t";
    }

    myfile << "\"id\"";
    myfile << field_delim;
	myfile << "\"sentiment\"";
	if (producir_piping){
		myfile << field_delim;
		myfile << "\"review\"";
	}
	myfile << row_delim;

	if (!producir_piping){
		std::string path("testData.tsv");
		std::vector<long double> preds = miPerceptron.Predicciones(path);
		std::vector<std::string> ids = miPerceptron.ObtenerIds();
		std::vector<std::string> reviews = miPerceptron.ObtenerReviews();
		for (int i = 0; i < 25000; i++){
			myfile << ids[i];
			myfile << field_delim;
			myfile << std::fixed << std::setprecision(53) << preds[i];

			if (producir_piping){
				myfile << field_delim;
				myfile << reviews[i];
			}

			myfile << row_delim;
			//std::cout << std::fixed << std::cout.precision(190) << preds[i] << std::endl;
		}
	}

	if (producir_piping){
		std::string path_50k("unlabeledTrainData.tsv");
		std::vector<long double> preds_50k = miPerceptron.Predicciones(path_50k);
		std::vector<std::string> reviews = miPerceptron.ObtenerReviews();
		std::vector<std::string> ids = miPerceptron.ObtenerIds();
		for (int i = 0; i < 50000; i++){
			myfile << ids[i];
			myfile << field_delim;
			myfile << std::fixed << std::setprecision(53) << preds_50k[i];
			myfile << field_delim;
			myfile << reviews[i];
			myfile << row_delim;
		}
	}

	myfile.close();

	return 0;

}
