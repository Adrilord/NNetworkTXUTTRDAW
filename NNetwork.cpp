#include "NNetwork.h"

NNetwork::NNetwork(vector<Layer> Layers) 
{
	this->Layers = Layers;
}

NNetwork::NNetwork(vector<int>& sizeLayers, double sigma)
{
	this->Layers.push_back(Layer(sizeLayers.at(0),sizeLayers.at(0)));
	for(unsigned int i=1; i<sizeLayers.size(); i++) {
		int nben=sizeLayers.at(i-1);
		int nbout=sizeLayers.at(i);
		this->Layers.push_back(Layer(nben,nbout,true,1.0));
	}
}

NNetwork::~NNetwork() 
{
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		this->Layers.at(i).freeSpace(); //(Utilisation du destructeur manuel)
	}
}

vector<double> NNetwork::calculOutput(vector<double> input)
{
	//CREATION
	gsl_vector* gslInput = stdToGslVector(input);  
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
		this->Layers.at(i).calculOutput(preOutputs[i], outputs[i], outputs[i-1]);
		this->Layers.at(i+1).calculPreOutput(outputs[i], preOutputs[i+1]);
	}
	unsigned int i=this->Layers.size()-1;
	this->Layers.at(i).calculOutput(preOutputs[i], outputs[i], outputs[i-1]);
	vector<double> stdOutput = gslToStdVector(outputs[i]);  
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
	return stdOutput;		
}

vector<vector<double>> NNetwork::calculOutput(vector<vector<double>> input)
{
	//CREATION
	gsl_matrix* gslInput = stdToGslMatrixTrans(input);  
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
		this->Layers.at(i).calculOutput(preOutputs[i], outputs[i], outputs[i-1]);
		this->Layers.at(i+1).calculPreOutput(outputs[i], preOutputs[i+1]);
	}
	unsigned int i=this->Layers.size()-1;
	this->Layers.at(i).calculOutput(preOutputs[i], outputs[i], outputs[i-1]);
	vector<vector<double>> stdOutput = gslToStdMatrix(outputs[i]);  
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
	return stdOutput;	
}

void NNetwork::trainNNetwork(vector<double> input, vector<double> expectedOutput, int costID, const double learningRate) 
{
	//CREATION
	gsl_vector* gslInput = stdToGslVector(input);  
	gsl_vector* gslExpectedOutput = stdToGslVector(expectedOutput);  
	gsl_vector* preOutputs[this->Layers.size()];
	gsl_vector* outputs[this->Layers.size()];
	gsl_vector* derivateOutputs[this->Layers.size()];
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		preOutputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
		outputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
		derivateOutputs[i] = gsl_vector_alloc(this->Layers.at(i).getNbOut());
	}
	//CALCULATION
	gsl_blas_dcopy(gslInput, preOutputs[0]);
	gsl_blas_dcopy(gslInput, outputs[0]);
	for(unsigned int i=1; i<this->Layers.size()-1; i++) { //i=0 est la couche input 
		this->Layers.at(i).calculOutput(preOutputs[i], outputs[i], outputs[i-1]);
		this->Layers.at(i).calculDerivateOutput(preOutputs[i], derivateOutputs[i]);
		this->Layers.at(i+1).calculPreOutput(outputs[i], preOutputs[i+1]);
	}
	unsigned int i=this->Layers.size()-1;
	this->Layers.at(i).calculOutput(preOutputs[i], outputs[i], outputs[i-1]);
	this->Layers.at(i).calculDerivateOutput(preOutputs[i], derivateOutputs[i]);
	vector<double> stdOutput = gslToStdVector(outputs[i]);  
	//BACKPROPAGATION -> ERRORS
	gsl_vector* errors[this->Layers.size()];
	//Output error
	errors[this->Layers.size()-1] = this->calculCostDerivate(outputs[this->Layers.size()-1], gslExpectedOutput, costID);
	gsl_vector_mul(errors[this->Layers.size()-1], derivateOutputs[this->Layers.size()-1]);
	//Backpropagation of the error
	for(unsigned int l=this->Layers.size()-2; l>=0; l++) {
		errors[l] = gsl_vector_alloc(this->Layers.at(l).getNbOut());
		this->Layers.at(l).calculDelta(errors[l], errors[l+1]);
		gsl_vector_mul(errors[l], derivateOutputs[l]);
	}
	//GRADIENT DESCENT (CORRECTIONS)
	for(unsigned int l=this->Layers.size()-1; l>=1; l++) {
		//corretionWeights=learningRate*errors[l]*trans(outputs[l-1])
		gsl_matrix* correctionWeights = gsl_matrix_alloc(errors[l]->size, outputs[l-1]->size);
		gsl_blas_dger(1.f, errors[l], outputs[l-1], correctionWeights);
		gsl_matrix_scale(correctionWeights, learningRate);
		this->Layers.at(l).correctWeights(correctionWeights);
		//correctionBias=learningRate*errors[l]
		gsl_vector* correctionBias = gsl_vector_alloc(this->Layers.at(l).getNbOut());
		gsl_blas_dcopy(errors[l], correctionBias);
		gsl_vector_scale(correctionBias, learningRate);
		this->Layers.at(l).correctBias(correctionBias);
		//DESTRUCTION
		gsl_matrix_free(correctionWeights);
		gsl_vector_free(correctionBias);
	}
	
	//DESTRUCTION
	gsl_vector_free(gslInput);
	gsl_vector_free(gslExpectedOutput);
	for(unsigned int i=0; i<this->Layers.size(); i++) {
		gsl_vector_free(preOutputs[i]);
		gsl_vector_free(outputs[i]);
		gsl_vector_free(derivateOutputs[i]);
		gsl_vector_free(errors[i]);
	}
}

gsl_vector* NNetwork::calculCostDerivate(gsl_vector* finalOutput, gsl_vector* expectedOutput, int costID)
{
	gsl_vector* result = gsl_vector_alloc(expectedOutput->size);
	switch (costID) {
		case QUADRATICCOST :
			gsl_blas_dcopy(expectedOutput, result);
			gsl_vector_sub(result,finalOutput);
			return result;
			break;
		/*case CROSSENTROPY :
			return ;
			break;*/
		default :
			gsl_blas_dcopy(expectedOutput, result);
			gsl_vector_sub(result,finalOutput);
			return result;
			break;
	}
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
