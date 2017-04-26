#include "Layer.h"

Layer::Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias, vector<int>& functionsID, vector<vector<double>>& functionsParam)
{
	this->nben=nben;
	this->nbout=nbout;
	this->weights=this->stdToGslMatrix(weights);
	this->bias=stdToGslVector(bias);
	this->functionsID=functionsID;
	this->functionsParam=functionsParam;
}

Layer::Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias)
{ //with simple sigmoïd functions
	this->nben=nben;
	this->nbout=nbout;
	this->weights=this->stdToGslMatrix(weights);
	this->bias=stdToGslVector(bias);
	for(int i=0; i<nbout; i++) {
		this->functionsID.push_back(SIGMOID);
	}
}

Layer::Layer(int& nben, int& nbout) 
{ // for input layer
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

void Layer::freeSpace()
{
	gsl_vector_free(this->bias);
	gsl_matrix_free(this->weights);
}

void Layer::calculPreOutput(gsl_vector* en, gsl_vector* preOutput) 
{
	gsl_blas_dcopy(this->bias, preOutput);
	gsl_blas_dgemv (CblasNoTrans, 1.f, this->weights, en, 1, preOutput);
}

void Layer::calculOuput(gsl_vector* preOutput, gsl_vector* output)
{
	for (int i = 0; i < this->nbout; i++) {
		double z = gsl_vector_get (preOutput, i); //présortie
		double a = calculFromFunction(i,z);//sortie
		gsl_vector_set (output, i, a);
	}
}

void Layer::calculDelta(gsl_vector* en, gsl_vector* delta, Layer nextLayer)
{
}

double Layer::calculFromFunction(int neuron, double& z)
{
	switch (functionsID.at(neuron)) {
		case SIGMOID :
			return 1.f /(1+gsl_sf_exp(-z));
			break;
		case ID :
			return z;
			break;
		default :
			return z;
			break;
	}
}

gsl_vector* Layer::stdToGslVector(vector<double>& stdVector)
{
	gsl_vector* gslvector = gsl_vector_alloc(stdVector.size());
	for(unsigned int i=0; i<stdVector.size(); i++) {
			gsl_vector_set (gslvector, i, stdVector.at(i));
	}
	return gslvector;
}


gsl_matrix* Layer::stdToGslMatrix(vector<vector<double>>& stdMatrix)
{
	gsl_matrix* gslmatrix = gsl_matrix_alloc (stdMatrix.size(), stdMatrix.at(0).size());
	for(unsigned int j=0; j<stdMatrix.size(); j++) {
		for(unsigned int k=0; k<stdMatrix.at(j).size(); k++) {
				gsl_matrix_set (gslmatrix, j, k, stdMatrix.at(j).at(k));
			}
		}
	return gslmatrix;
}


vector<double> Layer::gslToStdVector(gsl_vector* gslvector)
{
	vector<double> stdVector;
	for(unsigned int i=0; i<gslvector->size; i++) {
		stdVector.push_back(gsl_vector_get(gslvector, i));
	}
	return stdVector;
}


vector<vector<double>> Layer::gslToStdMatrix(gsl_matrix* gslmatrix)
{
	vector<vector<double>> stdMatrix;
	for(unsigned int j=0; j<gslmatrix->size1; j++) {
		vector<double> lign;
		for(unsigned int k=0; k<gslmatrix->size2; k++) {
				lign.push_back(gsl_matrix_get(gslmatrix, j, k));
		}
		stdMatrix.push_back(lign);
	}
	return stdMatrix;
}

int Layer::getNbEn()
{
	return this->nben;
}

int Layer::getNbOut()
{
	return this->nbout;
}

void Layer::printLayerInfo()
{
	cout << "nben : " << this->nben << " nbout : " << this->nbout << endl;
	cout << "weight values :" << endl;
	for(unsigned int j=0; j<this-> weights->size1; j++) {
		for(unsigned int k=0; k<this-> weights->size2; k++) {
				cout << gsl_matrix_get(this->weights, j, k) << " ";
		}
		cout << endl;
	}
	cout << "bias values :" << endl;
	for(unsigned int i=0; i<this->bias->size; i++) {
		cout << gsl_vector_get(this->bias, i) << endl;
	}
	cout << "functionID values :" << endl;
	for(unsigned int i=0; i<this->functionsID.size(); i++) {
		cout << this->functionsID.at(i) << endl;
	}
}
