#ifndef LAYER
#define LAYER

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
//Spécification XML (define des id de fonctions)
#include "xmlspec.h"

using namespace std;

//Classe représentant une couche d'un réseau de neurones (input, hidden,
//ou output)
class Layer {
	private :
		int nben; // nombre d'entrées
		int nbout; // nombre de sorties
		gsl_matrix* weights; // matrice des poids
		gsl_vector* bias; // vecteur biais d'une couche
		vector<int> functionsID; // ID des fonctions de chaque neurone
		vector<vector<double>> functionsParam; // Paramètres de chaque fonction de chaque neurone
		
	public :
		//Constructeurs de la classe
		Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias, vector<int>& functionsID, vector<vector<double>>& functionsParam);
		Layer(int& nben, int& nbout, vector<vector<double>>& weights, vector<double>& bias); //avec des fonctions SIGMOID simples
		Layer(int& nben, int& nbout); // utilisé pour l'input layer
		
		//Fonction qui libère weights et bias
		void freeSpace(); //A utiliser manuellement plutôt que d'avoir un destructeur (cela pose des problèmes)

		//Calculs de forwarding par étape
		//pour obtenir preOutput = weights*input+bias
		void calculPreOutput(gsl_vector* input, gsl_vector* preOutput);
		void calculPreOutput(gsl_matrix* input, gsl_matrix* preOutput); // plusieurs inputs à la fois
		//pour obtenir output = functionOfNeurons ( preOutput, input )
		//où input intervient dans les réseaux RBF notamment
		void calculOuput(gsl_vector* preOutput, gsl_vector* output, gsl_vector* input);
		void calculOuput(gsl_matrix* preOutput, gsl_matrix* output, gsl_matrix* input); // plusieurs inputs à la fois
		
		//TODO Fonction de calcul de backwarding
		void calculDelta(gsl_vector* en, gsl_vector* delta, Layer nextLayer); //backward propagation
		
		//Fonction de calcul pour chaque neurone
		double calculFromFunction(int neuron, double& z, gsl_vector* input);
		double calculFromFunction(int neuron, double& z, gsl_matrix* input);
		//Fonction utilisé dans le cas de réseaux RBF
		double calculDistForRBF(vector<double> params, gsl_vector* input);
		double calculDistForRBF(vector<double> params, gsl_matrix* input);
		
		//Fonctions de transformation de std::vector en objets gsl
		gsl_vector* stdToGslVector(vector<double>& stdVector);
		gsl_matrix* stdToGslMatrix(vector<vector<double>>& stdMatrix); //les vector sont en lignes
		gsl_matrix* stdToGslMatrixTrans(vector<vector<double>>& stdMatrix); //les vector sont en colonnes
		//Fonctions de transformation d'objets gsl en std::vector
		vector<double> gslToStdVector(gsl_vector* gslvector);
		vector<vector<double>> gslToStdMatrix(gsl_matrix* gslmatrix); //les vector sont en lignes
		vector<vector<double>> gslToStdMatrixTrans(gsl_matrix* gslmatrix); //les vector sont en colonnes
		
		//Randomizer des valeurs de poids et de biais selon une loi normale
		void randomizeGaussian(double moy, double sigma);
		
		//Fonctions qui retournent les attributs
		int getNbEn();
		int getNbOut();
		vector<double> getBias();
		vector<vector<double>> getWeights();
		vector<int> getFunctionsID();
		vector<vector<double>> getFunctionsParam();
		
		//Affichage des informations (valeurs des attributs) sur la
		//sortie standard
		void printLayerInfo();
};
#endif
