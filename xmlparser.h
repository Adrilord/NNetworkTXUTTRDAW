#ifndef XMLPARSER
#define XMLPARSER

#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <tinyxml.h>
#include "NNetwork.h"
#include "Layer.h"
#include "xmlspec.h"

using namespace std;

NNetwork xmlToNNetwork(string xmlfilename);

Layer xmlToLayer(TiXmlElement* firstNeuronElem, int nben, int layerTypeID, int nbout);

vector<double> decodeListOfDouble(string values);

void saveNetwork(NNetwork& network, string xmlfilename);

char* codeListOfDouble(vector<double>& listOfDouble);




#endif
