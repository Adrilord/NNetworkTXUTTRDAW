#ifndef NNETWORK
#define NNETWORK

#include <string>
#include <iostream>
#include <vector>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
#include "Layer.h"

using namespace std;

class NNetwork {
	private :
		vector<Layer> Layers; //couches (input et output comprises)
	public :
		NNetwork(vector<Layer> Layers);
		
		~NNetwork();
		
		vector<double> calculOuput(vector<double> input);
		vector<vector<double>> calculOuput(vector<vector<double>> input);
		
		//~ NNetwork trainNNetwork(vector<double> input);
		//~ NNetwork trainNNetwork(vector<double> input);
		
		void printNetworkInfo();
};

#endif
