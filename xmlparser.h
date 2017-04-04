#ifndef XMLPARSER
#define XMLPARSER

#include <string>
#include <iostream>
#include <tinyxml.h>
#include "NNetwork.h"
#include "Layer.h"

using namespace std;

class xmlparser {
	private :
		TiXmlDocument docNNetwork;
	
	public : 
		xmlpaser(string xmlfile);

		NNetwork readNNetwork();
		saveNetwork(string xmlfilename);
}

#endif
