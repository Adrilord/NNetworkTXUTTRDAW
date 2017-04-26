#include "NNetwork.h"

NNetwork::NNetwork(vector<Layer> Layers) 
{
	this->Layers = Layers;
}

NNetwork::~NNetwork() 
{
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		this->Layers.at(i).freeSpace();
	}
}

vector<double> NNetwork::calculOuput(vector<double> input)
{
	//CREATION
	gsl_vector* gslInput = this->Layers.at(0).stdToGslVector(input);
	gsl_vector* preOutputs[this->Layers.size()];
	gsl_vector* outputs[this->Layers.size()];
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		preOutputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
		outputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
	}
	//CALCULATION
	gsl_blas_dcopy(gslInput, preOutputs[0]);
	gsl_blas_dcopy(gslInput, outputs[0]);
	for(unsigned int i=0; i<this->Layers.size()-1; i++) {
		this->Layers.at(i).calculOuput(preOutputs[i], outputs[i]);
		this->Layers.at(i+1).calculPreOutput(outputs[i], preOutputs[i+1]);
	}
	unsigned int i=this->Layers.size()-1;
	this->Layers.at(i).calculOuput(preOutputs[i], outputs[i]);
	vector<double> stdOuput = this->Layers.at(0).gslToStdVector(outputs[i]);
	//TEST
	cout << "OUTPUTs" << endl;
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		cout << "OUTPUT n° : " << i << endl;
		for(unsigned int j=0; j<outputs[i]->size; j++) {
			cout << gsl_vector_get(outputs[i], j) << endl;
		}
	}
	//DESTRUCTION
	gsl_vector_free(gslInput);
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		gsl_vector_free(preOutputs[i]);
		gsl_vector_free(outputs[i]);
	}
	//RETURNATION
	return stdOuput;	
	//~ void calculPreOutput(gsl_vector* en, gsl_vector* preOutput);
	//~ void calculOuput(gsl_vector* preOutput, gsl_vector* output);	
}

vector<vector<double>> NNetwork::calculOuput(vector<vector<double>> input)
{
	
}
		
//gsl_matrix* calculOuput(gsl_matrix* input);
