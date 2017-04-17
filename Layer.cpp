#include "Layer.h"
#include <string>


Layer::Layer(int& nben, int& nbout, gsl_matrix* weights, gsl_vector* bias, vector<int>& functionsID, vector<vector<double>>& functionsParam) {
	this->nben=nben;
	this->nbout=nbout;
	this->weights=weights;
	this->bias=bias;
	this->functionsID=functionsID;
	this->functionsParam=functionsParam;
}

Layer::Layer(int& nben, int& nbout, gsl_matrix* weights, gsl_vector* bias) { //with sigmoïd functions
	this->nben=nben;
	this->nbout=nbout;
	this->weights=weights;
	this->bias=bias;
	for(int i=0; i<nbout; i++) {
		this->functionsID.push_back(SIGMOID);
	}
}

Layer::Layer(int& nben, int& nbout) { // for input layer
	this->nben=nben;
	this->nbout=nbout;
	this->weights = gsl_matrix_alloc (nbout, nben);
	for(int j=0; j<nbout; j++) {
		for(int k=0; k<nben; k++) {
			if(j==k) {
				gsl_matrix_set (this->weights, j, k, 1);
			} else {
				gsl_matrix_set (this->weights, j, k, 0);
			}
		}
	}
	this->bias=gsl_vector_alloc (nbout);
	for(int i=0; i<nbout; i++) {
		gsl_vector_set (this->bias, i, 0);
	}
	for(int i=0; i<nbout; i++) {
		this->functionsID.push_back(ID);
	}
}

Layer::~Layer() {
	gsl_matrix_free (this->weights);
	gsl_vector_free (this->bias);
}

gsl_vector* Layer::calculPreOutput(gsl_vector* en) {
	gsl_vector* preout = gsl_vector_alloc (this->nbout);
	preout = this->bias;
	gsl_blas_dgemv (CblasNoTrans, 1.f, this->weights, en, 1, preout);
	return preout;
}

gsl_vector* Layer::calculOuput(gsl_vector* preOutput) {
	gsl_vector* out = gsl_vector_alloc (this->nbout);
	for (int i = 0; i < this->nbout; i++)
    {
		double z = gsl_vector_get (preOutput, i); //présortie
		double a = calculFromFunction(this->functions[1],z);//sortie
		printf("z : %g    a: %g\n", z, a);
		gsl_vector_set (out, i, a);
    }
    return out;
}

double Layer::calculFromFunction(int functionID, vector<double>& functionsParam, double z) {
	if(functionID == SIGMOID) {
		return 1.f /(1+gsl_sf_exp(-z));
	} else if (functionID == ID) {
		return z;
	} else {
		return z;
	}
}
			
