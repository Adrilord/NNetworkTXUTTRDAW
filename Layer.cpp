#include "Layer.h"
#include <string>


Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias, vector<int>& functionsID, vector<vector<double>>& functionsParam) {
	this->nben=nben;
	this->nbout=nbout;
	this->weights=weights;
	this->bias=bias;
	this->functionsID=functionsID;
	this->functionsParam=functionsParam;
}

Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias){ //with sigmoïd functions
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
	/***TODO***/
	gsl_matrix* tempweights = gsl_matrix_alloc (nbout, nben);
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
	/*********/
	for(int i=0; i<nbout; i++) {
		this->functionsID.push_back(ID);
	}
}

Layer::~Layer() {
}

/***TODO***/
vector<double> Layer::calculPreOutput(vector<double> en) {
	gsl_vector* preout = gsl_vector_alloc (this->nbout);
	preout = this->bias;
	gsl_blas_dgemv (CblasNoTrans, 1.f, this->weights, en, 1, preout);
	return preout;
}

vector<double> Layer::calculOuput(vector<double> preOutput) {
	gsl_vector* out = gsl_vector_alloc (this->nbout);
	for (int i = 0; i < this->nbout; i++)
    {
		double z = gsl_vector_get (preOutput, i); //présortie
		double a = calculFromFunction(this->functionsID[1], this->functionsParam.at(i),z);//sortie
		printf("z : %g    a: %g\n", z, a);
		gsl_vector_set (out, i, a);
    }
    return out;
}

double Layer::calculFromFunction(int functionID, vector<double>& functionsParam, double& z) {
	if(functionID == SIGMOID) {
		return 1.f /(1+gsl_sf_exp(-z));
	} else if (functionID == ID) {
		return z;
	} else {
		return z;
	}
}

gsl_vector* stdToGslVector(vector<double>& stdVector) {
	gsl_vector* gslvector = gsl_vector_alloc(stdVector.size());
	for(int i=0; i<stdVector.size(); i++) {
			gsl_vector_set (gslvector, i, tempbias.at(i));
	}
	return gslvector;
}


gsl_matrix* stdToGslMatrix(vector<vector<double>>& stdMatrix) {
	gsl_matrix* gslmatrix = gsl_matrix_alloc (stdMatrix.size(), stdMatrix.at(j).size());
	for(int j=0; j<stdMatrix.size(); j++) {
		for(int k=0; k<stdMatrix.at(j).size(); k++) {
				gsl_matrix_set (gslmatrix, j, k, stdMatrix.at(j).at(k));
			}
		}
	return gslmatrix;
}


vector<double> gslVectorToStd(gsl_vector* gslvector) {
	vector<double> stdVector;
	for(int i=0; i<gslvector.size; i++) {
		stdVector.push_back(gsl_vector_get(gslvector, i));
	}
}


vector<vector<double>> gslMatrixToStd(gsl_vector* gslmatrix) {
	vector<vector<double>> stdMatrix;
	for(int j=0; j<gslmatrix.size1; j++) {
		vector<double> lign;
		for(int k=0; k<gslmatrix.size2; k++) {
				lign.push_back(gsl_matrix_get(gslmatrix, j, k));
			}
		stdMatrix.push(lign);
	}
}
			
/********/
