#include "Layer.h"

Layer::Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias, vector<int>& functionsID, vector<vector<double>>& functionsParam)
{
	this->nben=nben;
	this->nbout=nbout;
	this->weights=stdToGslMatrix(weights);
	this->bias=stdToGslVector(bias);
	this->functionsID=functionsID;
	this->functionsParam=functionsParam;
}

Layer::Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias)
{ //with simple sigmoïd functions
	this->nben=nben;
	this->nbout=nbout;
	this->weights=stdToGslMatrix(weights);
	this->bias=stdToGslVector(bias);
	for(int i=0; i<nbout; i++) {
		this->functionsID.push_back(SIGMOID);
	}
}

Layer::Layer(int& nben, int& nbout, bool randomizeGaussian, double sigma, vector<int>& functionsID, vector<vector<double>>& functionsParam) 
{
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
	this->functionsID=functionsID;
	this->functionsParam=functionsParam;
	if(randomizeGaussian) {
		this->randomizeGaussian(sigma);
	}
	if(randomizeGaussian) {
		this->randomizeGaussian(sigma);
	}
}

Layer::Layer(int& nben, int& nbout, bool randomizeGaussian, double sigma)
{
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
		this->functionsID.push_back(SIGMOID);
	}
	if(randomizeGaussian) {
		this->randomizeGaussian(sigma);
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

void Layer::calculPreOutput(gsl_vector* input, gsl_vector* preOutput) 
{
	gsl_blas_dcopy(this->bias, preOutput);
	gsl_blas_dgemv (CblasNoTrans, 1.f, this->weights, input, 1, preOutput);
}

void Layer::calculPreOutput(gsl_matrix* input, gsl_matrix* preOutput) 
{
	for(unsigned int j=0; j<preOutput->size1; j++) {
		for(unsigned int k=0; k<preOutput->size2; k++) {
				gsl_matrix_set(preOutput, j, k, gsl_vector_get(this->bias, j));
		}
	}
	gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.f, this->weights, input, 1, preOutput);
}

void Layer::calculOutput(gsl_vector* preOutput, gsl_vector* output, gsl_vector* input)
{
	for (int i = 0; i < this->nbout; i++) {
		double z = gsl_vector_get(preOutput, i); //présortie
		double a = calculFromFunction(i,z,input);//sortie
		gsl_vector_set (output, i, a);
	}
}

void Layer::calculOutput(gsl_matrix* preOutput, gsl_matrix* output, gsl_matrix* input)
{
	for(unsigned int j=0; j<preOutput->size1; j++) {
		for(unsigned int k=0; k<preOutput->size2; k++) {
			double z = gsl_matrix_get(preOutput, j, k); //présortie
			gsl_vector* tempInput = gsl_vector_alloc(input->size1);
			for(unsigned int i=0; i<tempInput->size; i++) {
				gsl_vector_set(tempInput, i, gsl_matrix_get(input, i, k));
			}
			double a = calculFromFunction(j,z,tempInput);//sortie //TODO à adapter pour RBF
			gsl_vector_free(tempInput);
			gsl_matrix_set (output, j, k, a);
		}
	}
}

void Layer::calculDerivateOutput(gsl_vector* preOutput, gsl_vector* output)
{
	for (int i = 0; i < this->nbout; i++) {
		double z = gsl_vector_get(preOutput, i); //présortie
		double a = calculFromFunctionDerivate(i,z);//sortie
		gsl_vector_set (output, i, a);
	}
}

double Layer::calculFromFunction(int neuron, double& z, gsl_vector* input)
{
	vector<double> params;
	if(this->functionsParam.size() >= 1) {
			params = this->functionsParam.at(neuron);
	}
	switch (functionsID.at(neuron)) {
		case ID :
			return z;
			break;
		case SIGMOID :
			return 1.f /(1+gsl_sf_exp(-z));
			break;
		case SIGMOIDP :
			return 1.f /(1+gsl_sf_exp(-params.at(0)*z));
			break;
		case TANH : 
			return (gsl_sf_exp(z) - gsl_sf_exp(-z)) / (gsl_sf_exp(z) + gsl_sf_exp(-z));
			break;
		case GAUSSIAN :
			return gsl_sf_exp(-params.at(0) * calculDistForRBF(params, input));
			break;
		default :
			return z;
			break;
	}
}

double Layer::calculFromFunctionDerivate(int neuron, double& z /*, gsl_vector */)
{
	vector<double> params;
	if(this->functionsParam.size() >= 1) {
			params = this->functionsParam.at(neuron);
	}
	switch (functionsID.at(neuron)) {
		case SIGMOID :
			return gsl_sf_exp(-z) /((1+gsl_sf_exp(-z))*(1+gsl_sf_exp(-z)));
			break;
		case SIGMOIDP :
			return (params.at(0)* gsl_sf_exp(-params.at(0)*z)) /((1+gsl_sf_exp(-params.at(0)*z))*(1+gsl_sf_exp(-params.at(0)*z)));
			break;
		default :
			return z;
			break;
	}
}

double Layer::calculDistForRBF(vector<double> params, gsl_vector* input)
{
	gsl_vector* center = gsl_vector_alloc(input->size);
	for(unsigned int i=0; i < center->size; i++) {
		gsl_vector_set(center, i, params.at(i+1));
	}
	gsl_vector* diff = center;
	gsl_vector_sub(diff, input);
	
	double z = 0;
	gsl_blas_ddot(diff, diff, &z);
	
	gsl_vector_free(diff); //libére aussi center, ils pointent sur le mêm objet
	return z;
}

void Layer::randomizeGaussian(double sigma)
{
	const gsl_rng_type * T;
	gsl_rng * r;

	gsl_rng_env_setup();

	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	for(unsigned int j=0; j<this->weights->size1; j++) {
		for(unsigned int k=0; k<this->weights->size2; k++) {
			gsl_matrix_set (this->weights, j, k, gsl_ran_gaussian (r, sigma));
		}
	}
	for(unsigned int i=0; i<this->bias->size; i++) {
		gsl_vector_set (this->bias, i, gsl_ran_gaussian (r, sigma));
	}

	gsl_rng_free (r);
}

int Layer::getNbEn()
{
	return this->nben;
}

int Layer::getNbOut()
{
	return this->nbout;
}

vector<double> Layer::getBias()
{
	return gslToStdVector(this->bias);
}

vector<vector<double>> Layer::getWeights()
{
	return gslToStdMatrix(this->weights);
}

vector<int> Layer::getFunctionsID()
{
	return this->functionsID;
}

vector<vector<double>> Layer::getFunctionsParam()
{
	return this->functionsParam;
}

void Layer::calculDelta(gsl_vector* currentError, gsl_vector* previousError)
{
	gsl_vector_set_zero(currentError);
	gsl_blas_dgemv(CblasTrans, 1.f, this->weights, previousError, 1.f, currentError);
}

void Layer::correctBias(gsl_vector* correction) 
{
	gsl_vector_sub(this->bias,correction);
}

void Layer::correctWeights(gsl_matrix* correction) 
{
	gsl_matrix_sub(this->weights,correction);
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
	cout << "functionParameters values :" << endl;
	for(unsigned int j=0; j < this->functionsParam.size(); j++) {
		for(unsigned int k=0; k < this->functionsParam.at(j).size(); k++) {
				cout << functionsParam.at(j).at(k) << " ";
		}
		cout << endl;
	}
}

gsl_vector* stdToGslVector(vector<double>& stdVector)
{
	gsl_vector* gslvector = gsl_vector_alloc(stdVector.size());
	for(unsigned int i=0; i<stdVector.size(); i++) {
			gsl_vector_set (gslvector, i, stdVector.at(i));
	}
	return gslvector;
}

gsl_matrix* stdToGslMatrix(vector<vector<double>>& stdMatrix) //les vector sont en ligne
{
	gsl_matrix* gslmatrix = gsl_matrix_alloc (stdMatrix.size(), stdMatrix.at(0).size());
	for(unsigned int j=0; j<stdMatrix.size(); j++) {
		for(unsigned int k=0; k<stdMatrix.at(j).size(); k++) {
				gsl_matrix_set (gslmatrix, j, k, stdMatrix.at(j).at(k));
			}
	}
	return gslmatrix;
}

gsl_matrix* stdToGslMatrixTrans(vector<vector<double>>& stdMatrix) //les vector sont en colonnes
{
	gsl_matrix* gslmatrix = gsl_matrix_alloc (stdMatrix.at(0).size(), stdMatrix.size());
	for(unsigned int k=0; k<stdMatrix.size(); k++) {
		for(unsigned int j=0; j<stdMatrix.at(k).size(); j++) {
				gsl_matrix_set (gslmatrix, j, k, stdMatrix.at(k).at(j));
			}
	}
	return gslmatrix;
}


vector<double> gslToStdVector(gsl_vector* gslvector)
{
	vector<double> stdVector;
	for(unsigned int i=0; i<gslvector->size; i++) {
		stdVector.push_back(gsl_vector_get(gslvector, i));
	}
	return stdVector;
}

vector<vector<double>> gslToStdMatrix(gsl_matrix* gslmatrix) //les vector sont en lignes
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

vector<vector<double>> gslToStdMatrixTrans(gsl_matrix* gslmatrix) //les vector sont en colonnes
{
	vector<vector<double>> stdMatrix;
	for(unsigned int k=0; k<gslmatrix->size2; k++) {
		vector<double> column;
		for(unsigned int j=0; j<gslmatrix->size1; j++) {
				column.push_back(gsl_matrix_get(gslmatrix, j, k));
		}
		stdMatrix.push_back(column);
	}
	return stdMatrix;
}
