#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
#include <cstdio>
#include "Layer.h"
#include "NNetwork.h"
#include "xmlparser.h"

void test0Layer () {
	int nben=10;
	int nbout=10;
	Layer lol = Layer(nben,nbout);
	gsl_vector* en = gsl_vector_alloc(nben);
	for(int i=0; i<nben; i++) {
		gsl_vector_set (en, i, 5);
	}
	gsl_vector* testout = gsl_vector_alloc(nbout);
	testout = lol.calculPreOutput(en);
	testout = lol.calculOuput(testout);
	for(int i=0; i<nbout; i++) {
		printf ("vout_%d = %g\n", i, gsl_vector_get (testout, i));
	}
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

void test0xmlparser() {
	NNetwork nono = xmlToNNetwork("testxml.xml");
}

int main(int argc, char* argv[]) {
	test0xmlparser();
	return 0;
}
