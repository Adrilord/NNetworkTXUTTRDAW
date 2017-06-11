#ifndef IOMANAGER
#define IOMANAGER

//Librairies standard
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <tinyxml.h>
#include <fstream>
#include <random>
#include <algorithm>
//Librairies locales
#include "NNetwork.h"
#include "Layer.h"
#include "xmlspec.h"
#include "xmlparser.h"

/*Fonction qui lit un fichier texte avec des valeurs séparés par un
 * espace et des retour \n, une ligne correspondant à un input
 * et qui retourne ces valeurs sour le format vector<vector<double> > */
vector<vector<double> > getInputsFromFile(string filename);

//Fonction qui écrit les valeurs d'un vector<vector<double> > dans fichier
void putOutputToFile(string filename, vector<vector<double> > outputs);

//Fonctions pour la lecture d'un fichier MNIST de test

//Passe une valeur 32bit de BSM à MSB (ATTENTION il faudrait utiliser uint32_t)
int ReverseInt (int i);

//Lit le fichier d'images d'entrainement de MNIST pour l'obtenir 
// "au format" vector<vector<double> > exploitable
void ReadMNISTTrainingImages(int NumberOfImages, int DataOfAnImage,vector<vector<double> > &arr);

//Lit le fichier des labels d'entrainement de MNIST pour l'obtenir 
// "au format" vector<double> exploitable
void ReadMNISTTrainingLabels(int NumberOfItems, vector<double> &arr);

//Permet d'extraire un mini-batch avec une sélection aléatoire de batchsize
//items
void extractingAMNISTMiniBatch(vector<vector<double> > &images, vector<double> &labels, 
	vector<vector<double> > &minibatchimages, vector<double> &minibatchlabels, unsigned int batchsize);

//Fonctions d'exploitation des labels (lien entre label et résultat attendu)
void label2MNISTExpectedOutput(double &label, vector<double> &expectedOutput);
void label2MNISTExpectedOutput(vector<double> &labels, vector<vector<double> > &expectedOutputs);
void outputToLabel(double &label, vector<double> &output);
void outputToLabels(vector<double> &labels, vector<vector<double> > &outputs);

#endif
