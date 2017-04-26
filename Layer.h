#ifndef LAYER
#define LAYER

#include <string>
#include <iostream>
#include <vector>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
#include "xmlspec.h"

using namespace std;

class Layer {
	private :
		int nben;
		int nbout;
		gsl_matrix* weights;
		gsl_vector* bias;
		vector<int> functionsID;
		vector<vector<double>> functionsParam;
	public :
		Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias, vector<int>& functionsID, vector<vector<double>>& functionsParam);
		Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias); //with sigmoïd functions
		Layer(int& nben, int& nbout); // for input layer
		
		void freeSpace(); //A utiliser manuellement plutôt que d'avoir un destructeur (cela pose des problèmes)

		void calculPreOutput(gsl_vector* en, gsl_vector* preOutput);
		void calculOuput(gsl_vector* preOutput, gsl_vector* output);
		void calculDelta(gsl_vector* en, gsl_vector* delta, Layer nextLayer); //backward propagation
		
		double calculFromFunction(int neuron, double& z);
		
		gsl_vector* stdToGslVector(vector<double>& stdVector);
		gsl_matrix* stdToGslMatrix(vector<vector<double>>& stdMatrix);
		
		vector<double> gslToStdVector(gsl_vector* gslvector);
		vector<vector<double>> gslToStdMatrix(gsl_matrix* gslmatrix);
		
		int getNbEn();
		int getNbOut();
		
		void printLayerInfo();
};
#endif
