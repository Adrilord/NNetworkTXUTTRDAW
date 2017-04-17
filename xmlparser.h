#ifndef XMLPARSER
#define XMLPARSER

#include <string>
#include <vector>
#include <iostream>
#include <tinyxml.h>
#include "NNetwork.h"
#include "Layer.h"
#include "xmlspec.h"

using namespace std;

NNetwork xmlToNNetwork(string xmlfilename);

Layer xmlToLayer(TiXmlElement* layerElement);

vector<int> decodeListOfInt(char* values);

void saveNetwork(NNetwork& network, string xmlfilename);

char* codeListOfInt(vector<int>& listOfInt) {




#endif
