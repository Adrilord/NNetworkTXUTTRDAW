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

vector<vector<double>> getInputsFromFile(string filename);

void putOutputToFile(string filename, vector<vector<double>> outputs);

int ReverseInt (int i);

void ReadMNIST(int NumberOfImages, int DataOfAnImage,vector<vector<double>> &arr);

#endif
