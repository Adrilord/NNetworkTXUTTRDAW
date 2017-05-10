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
//Librairies locales
#include "NNetwork.h"
#include "Layer.h"
#include "xmlspec.h"
#include "xmlparser.h"

/*Fonction qui lit un fichier texte avec des valeurs séparés par un
 * espace et des retour \n, une ligne correspondant à un input
 * et qui retourne ces valeurs sour le format vector<vector<double>> */
vector<vector<double>> getInputsFromFile(string filename);

//Fonction qui écrit les valeurs d'un vector<vector<double>> dans fichier
void putOutputToFile(string filename, vector<vector<double>> outputs);

//Fonctions pour la lecture d'un fichier MNIST de test

int ReverseInt (int i);

void ReadMNIST(int NumberOfImages, int DataOfAnImage,vector<vector<double>> &arr);

#endif
