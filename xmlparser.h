#ifndef XMLPARSER
#define XMLPARSER

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

using namespace std;

//Fonction qui à partir du fichier xml génère le réseau de neurones
//celui-ci selon la spécification donnée dans xmlspec.h
NNetwork xmlToNNetwork(string  xmlfilename);

//Fonction qui à partir d'un élement xml de Layer, génère une couche
Layer xmlToLayer(TiXmlElement* firstNeuronElem, int nben, int layerTypeID, int nbout);

//Fonction qui transforme une liste de double (nombres) au format texte
//au format vector<double>
vector<double> decodeListOfDouble(string values);

//Fonction qui à partir d'un objet NNetwork (réseau de neurones), génère
//un fichier xml de description de celui-ci selon la spécification
//donnée dans xmlspec.h
void saveNetwork(NNetwork& network, const char* xmlfilename);

//Fonction qui transforme une liste de double (nombres) au format
//vector<double> au format texte
char* codeListOfDouble(vector<double>& listOfDouble);

#endif
