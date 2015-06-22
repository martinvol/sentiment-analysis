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

typedef std::vector<std::vector<std::string> > Rows;


int main(int argc, char const *argv[]){
	std::ifstream input1("tronBayes.csv");
	std::ifstream input2("tron.csv");
	std::ifstream input3("OnlyBayes.csv");

	char const row_delim = '\n';
	char const field_delim = ',';
	std::string firstrow;

	Rows rows1;
	Rows rows2;
	Rows rows3;

	/*if (!input) {
		std::cout << "unable to load file" << std::endl;
	}*/

	std::ofstream myfile ("submission.csv", std::ofstream::trunc);
	myfile << "\"id\",\"sentiment\"";
	myfile << "\n";
	getline(input1, firstrow, row_delim);
	getline(input2, firstrow, row_delim);
	getline(input3, firstrow, row_delim);

	//std::string row3,row2;
	for (std::string row; getline(input1, row, row_delim); ) {

		rows1.push_back(Rows::value_type());
		std::istringstream ss(row);
		for (std::string field; getline(ss, field, field_delim); ) {
			rows1.back().push_back(field);
		}

		std::string row2; getline(input2, row2, row_delim);
		rows2.push_back(Rows::value_type());
		std::istringstream ss1(row2);
		for (std::string field; getline(ss1, field, field_delim); ) {
			rows2.back().push_back(field);
		}

		std::string row3; getline(input3, row3, row_delim);
		rows3.push_back(Rows::value_type());
		std::istringstream ss2(row3);
		for (std::string field; getline(ss2, field, field_delim); ) {
			rows3.back().push_back(field);
		}




	}
	for (unsigned int i = 0; i < rows1.size(); i++){
		long double r1 = std::stold(rows1[i][1].c_str());
		long double r2 = std::stold(rows2[i][1].c_str());
		long double r3 = std::stold(rows3[i][1].c_str());

		long double promedio = (r1 + r2 + r3)/ (long double) 3;

		myfile << rows1[i][0].c_str();

		myfile << ",";
		myfile << promedio;
		myfile << "\n";
		
	}

	input1.close();
	input2.close();
	input3.close();
	myfile.close();
	return 0;
}


