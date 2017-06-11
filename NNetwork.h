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

//Définitions pour le calcul de gradient de coût
#define QUADRATICCOST 0
#define CROSSENTROPY 1

//Classe réprésentant le réseau neuronal comme un ensemble de couche
//de neurones
class NNetwork {
	private :
		vector<Layer> Layers; //couches (input et output comprises)
		
	public :
		//Constructeur de la classe "classique"
		NNetwork(vector<Layer> Layers);
		//construction avec SIGMOID comme fonciton de neurone et génération aléatoire des poids
		NNetwork(vector<int>& sizeLayers, double sigma);
		
		//Destructeur
		~NNetwork();
		
		//Calculs de Forwarding
		//pour 1 input
		vector<double> calculOutput(vector<double> input);
		//pour plusieurs inputs
		vector<vector<double>> calculOutput(vector<vector<double>> input);
		
		//Fonctions pour l'apprentissages implémentant l'algorithme de rétropropagation d'erreurs
		void trainNNetwork(vector<double> input, vector<double> expectedOutput, int costID, const double learningRate);
		void trainNNetwork(vector<vector<double>> input, vector<vector<double>> expectedOutput, int costID, const double learningRate);
		
		//Fonctions pour calculer le gradient du coût (pour l'apprentissage) 
		//pour les variables de sorties
		gsl_vector* calculCostDerivate(gsl_vector* finalOutput, gsl_vector* expectedOutput, int costID);
		gsl_matrix* calculCostDerivate(gsl_matrix* finalOutput, gsl_matrix* expectedOutput, int costID);
		
		//Fonction qui retourne les attributs
		vector<Layer> getLayers();
		
		//Affichage des informations de chaque couche sur la sortie 
		//standard grâce aux printLayerInfo() 
		void printNetworkInfo();
};

#endif
