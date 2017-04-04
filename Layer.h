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

using namespace std;

class Layer {
	private :
		int nben;
		int nbout;
		gsl_matrix* weights;
		gsl_vector* bias;
		vector<string> functions;
	public :
		Layer(int& nben, int& nbout, gsl_matrix* weights, gsl_vector* bias, vector<string> functions);
		Layer(int& nben, int& nbout, gsl_matrix* weights, gsl_vector* bias); //with sigmoïd functions
		Layer(int& nben, int& nbout); // for input layer
		
		~Layer();

		gsl_vector* calculPreOutput(gsl_vector* en);
		gsl_vector* calculOuput(gsl_vector* preOutput);
		//gsl_vector calculDelta(gsl_vector* en, Layer nextLayer);
		
		double calculFromFunction(string function, double z);
};
#endif
