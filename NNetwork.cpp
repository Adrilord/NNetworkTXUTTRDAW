#include "NNetwork.h"

NNetwork::NNetwork(vector<Layer> Layers) 
{
	this->Layers = Layers;
}

NNetwork::~NNetwork() 
{
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		this->Layers.at(i).freeSpace(); //(Utilisation du destructeur manuel)
	}
}

vector<double> NNetwork::calculOuput(vector<double> input)
{
	//CREATION
	gsl_vector* gslInput = this->Layers.at(0).stdToGslVector(input);  //TODO accéder à la fonction sans objet ???
	gsl_vector* preOutputs[this->Layers.size()];
	gsl_vector* outputs[this->Layers.size()];
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		preOutputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
		outputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
	}
	//CALCULATION
	gsl_blas_dcopy(gslInput, preOutputs[0]);
	gsl_blas_dcopy(gslInput, outputs[0]);
	for(unsigned int i=1; i<this->Layers.size()-1; i++) { //i=0 est la couche input 
		this->Layers.at(i).calculOuput(preOutputs[i], outputs[i], outputs[i-1]);
		this->Layers.at(i+1).calculPreOutput(outputs[i], preOutputs[i+1]);
	}
	unsigned int i=this->Layers.size()-1;
	this->Layers.at(i).calculOuput(preOutputs[i], outputs[i], outputs[i-1]);
	vector<double> stdOuput = this->Layers.at(0).gslToStdVector(outputs[i]);  //TODO accéder à la fonction sans objet ???
	//TEST (marche)
	//~ cout << "OUTPUTs" << endl;
	//~ for(unsigned int i=0; i<this->Layers.size(); i++) {
		//~ cout << "OUTPUT n° : " << i << endl;
		//~ for(unsigned int j=0; j<outputs[i]->size; j++) {
			//~ cout << gsl_vector_get(outputs[i], j) << endl;
		//~ }
	//~ }
	//DESTRUCTION
	gsl_vector_free(gslInput);
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		gsl_vector_free(preOutputs[i]);
		gsl_vector_free(outputs[i]);
	}
	//RETURNATION
	return stdOuput;		
}

vector<vector<double>> NNetwork::calculOuput(vector<vector<double>> input)
{
	//CREATION
	gsl_matrix* gslInput = this->Layers.at(0).stdToGslMatrixTrans(input);  //TODO accéder à la fonction sans objet ???
	gsl_matrix* preOutputs[this->Layers.size()];
	gsl_matrix* outputs[this->Layers.size()];
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		preOutputs[i] = gsl_matrix_alloc(this->Layers.at(i).getNbOut(),gslInput->size2);
		outputs[i] = gsl_matrix_alloc(this->Layers.at(i).getNbOut(),gslInput->size2);
	}
	//CALCULATION
	gsl_matrix_memcpy(preOutputs[0], gslInput);
	gsl_matrix_memcpy(outputs[0], gslInput);
	for(unsigned int i=1; i<this->Layers.size()-1; i++) { //i=0 est la couche input 
		this->Layers.at(i).calculOuput(preOutputs[i], outputs[i], outputs[i-1]);
		this->Layers.at(i+1).calculPreOutput(outputs[i], preOutputs[i+1]);
	}
	unsigned int i=this->Layers.size()-1;
	this->Layers.at(i).calculOuput(preOutputs[i], outputs[i], outputs[i-1]);
	vector<vector<double>> stdOuput = this->Layers.at(0).gslToStdMatrix(outputs[i]);  //TODO accéder à la fonction sans objet ???
	//TEST (marche)
	//~ cout << "OUTPUTs" << endl;
	//~ for(unsigned int i=0; i<this->Layers.size(); i++) {
		//~ cout << "OUTPUT n° : " << i << endl;
		//~ for(unsigned int j=0; j<outputs[i]->size1; j++) {
			//~ for(unsigned int k=0; k<outputs[i]->size2; k++) {
				//~ cout << gsl_matrix_get(outputs[i], j, k) << " ";
			//~ }
			//~ cout << endl;
		//~ }
	//~ }
	//DESTRUCTION
	gsl_matrix_free(gslInput);
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		gsl_matrix_free(preOutputs[i]);
		gsl_matrix_free(outputs[i]);
	}
	//RETURNATION
	return stdOuput;	
}

vector<Layer> NNetwork::getLayers() 
{
	return this->Layers;
}

void NNetwork::printNetworkInfo() 
{
	for(unsigned int i=0; i < this->Layers.size(); i++) {
		cout << "Layer n° " << i << endl;
		this->Layers.at(i).printLayerInfo();
	}
}
