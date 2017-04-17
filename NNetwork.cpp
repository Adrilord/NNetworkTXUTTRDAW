#include "NNetwork.h"

NNetwork::NNetwork(vector<Layer> Layers) {
	this->Layers = Layers;
}

NNetwork::~NNetwork() {
	
}

gsl_vector* NNetwork::calculOuput(gsl_vector* input) {
	gsl_vector* out = input;
	vector<Layer>::iterator i;
	for(i=this->Layers.begin(); i!=this->Layers.end(); i++) {
		gsl_vector* temp = i->calculPreOutput(out);
		temp=calculOuput(temp);
		out=temp;
	}
	return out;
}
		
		
//gsl_matrix* calculOuput(gsl_matrix* input);
