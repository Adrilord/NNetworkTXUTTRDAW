#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
#include <cstdio>
#include "Layer.h"
#include "NNetwork.h"
#include "xmlparser.h"

void showGslVector(gsl_vector* gslVector)
{
	for(unsigned int i=0; i<gslVector->size; i++) {
		cout << "value " << i << " : " << gsl_vector_get(gslVector, i) << endl;
	}
}

void testLayer () 
{
	int nben=10;
	int nbout=5;
	Layer tlayer = Layer(nben,nbout);
	tlayer.printLayerInfo();
	gsl_vector* en = gsl_vector_alloc(nben);
	for(int i=0; i<nben; i++) {
		gsl_vector_set (en, i, 5);
	}
	gsl_vector* preout = gsl_vector_alloc(nbout);
	gsl_vector* out = gsl_vector_alloc(nbout);
	tlayer.calculPreOutput(en,preout);
	tlayer.calculOuput(preout,out);
	showGslVector(out);
}

void testtinyxml() {
	TiXmlDocument doc("testxml.xml");
	if(doc.LoadFile())
	{
		TiXmlHandle hDoc(&doc);
		TiXmlElement *pRoot, *pLayer, *pNeuron;
		pRoot = doc.FirstChildElement("NNetwork");
		if(pRoot)
		{
			pLayer = pRoot->FirstChildElement("Layer");
				if (pLayer)
				{
					pNeuron = pLayer->FirstChildElement("Neuron");
					if(pNeuron)
					{
						cout << pNeuron->Attribute("weights") << endl;
					}
				}
		}
	}
	else 
	{
		cout << "PB LECTURE" << endl;
	}
}

void testxmlparser() 
{
	NNetwork nono = xmlToNNetwork("testxml.xml");
}

void testForwarding()
{
	//Création et test du NNetwork
	NNetwork nono = xmlToNNetwork("testxml.xml");
	nono.printNetworkInfo();
	
	//Construction de l'input
	vector<double> input;
	input.push_back(4);
	input.push_back(5);
	input.push_back(2);

	//Calcul et test de l'output
	vector<double> output=nono.calculOuput(input);
	for(unsigned int i=0; i<output.size(); i++) {
		cout << "OUTPUT" << endl;
		cout << output.at(i) << endl;
	}
}

void testSaveNetwork()
{
	//Création et test du NNetwork
	NNetwork nono = xmlToNNetwork("testxml.xml");
	nono.printNetworkInfo();
	
	saveNetwork(nono, "savetestxml.xml");
}

int main(int argc, char* argv[]) {
	testSaveNetwork();
	return 0;
}
