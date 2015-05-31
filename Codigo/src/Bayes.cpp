
#include "Bayes.h"

/*
bool check_positive_negative(review, data){
    total = 0
    for w in review.split():
        proba =  float(data[w][0])/(float(data[w][0]) + float(data[w][1]))
        total += proba
    return 1 if total/float(len(review.split())) >= 0.5 else 0
}*/

void Bayes::entrenar(int tag, std::vector<std::string*>* oracion){
	std::string* palabra;
	
	for (unsigned int i = 0; i < oracion->size() ; i++){

		
		palabra = (*oracion)[i];
		//std::cout << palabra << "\n";
		

		if(data.find(*palabra) == data.end()){
			data[*palabra] = new std::vector<int>(2);
			(*data[*palabra])[0] = 0;
			(*data[*palabra])[1] = 0;
			
		}
		
		if(tag > 0){
			(*data[*palabra])[0]++;
		}else{
			(*data[*palabra])[1]++;
		}
		
	}
}

double evaluar (std::vector<std::vector<std::string*>*>* oraciones);