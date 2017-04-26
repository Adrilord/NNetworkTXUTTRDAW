#include "NNetwork.h"

NNetwork::NNetwork(vector<Layer> Layers) 
{
	this->Layers = Layers;
}

NNetwork::~NNetwork() 
{
	
}

vector<double> calculOuput(vector<double> input)
{
	//~ vector<gsl_vector*> outputs;
	
}

vector<vector<double>> calculOuput(vector<vector<double>> input)
{
	
}
		
//gsl_matrix* calculOuput(gsl_matrix* input);
