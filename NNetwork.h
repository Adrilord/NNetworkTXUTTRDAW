#ifndef NNETWORK
#define NNETWORK

//Librairies standards
#include <string>
#include <iostream>
#include <vector>
//Librairies GSL
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
//Classe Layer représentant une couche
#include "Layer.h"

using namespace std;

#define QUADRATICCOST 0
#define CROSSENTROPY 1

//Classe réprésentant le réseau neuronale comme un ensemble de couche
//de neurones
class NNetwork {
	private :
		vector<Layer> Layers; //couches (input et output comprises)
		
	public :
		//Constructeur de la classe
		NNetwork(vector<Layer> Layers);
		NNetwork(vector<int>& sizeLayers, double sigma); //avec SIGMOID et génération aléa
		
		//Destructeur
		~NNetwork();
		
		//Calculs de Forwarding
		//pour 1 input
		vector<double> calculOuput(vector<double> input);
		//pour plusieurs inputs
		vector<vector<double>> calculOuput(vector<vector<double>> input);
		
		NNetwork trainNNetwork(vector<double> input, vector<double> expectedOutput, int costID);
		//~ NNetwork trainNNetwork(vector<vector<double>> input, vector<vector<double>> expectedOutput);
		
		gsl_vector* calculCostDerivate(gsl_vector* finalOuput, gsl_vector* expectedOutput, int costID);
		
		//Fonction qui retourne les attributs
		vector<Layer> getLayers();
		
		//Affichage des informations de chaque couche sur la sortie 
		//standard grâce aux printLayerInfo() 
		void printNetworkInfo();
};

#endif
