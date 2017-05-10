#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
#include <cstdio>
#include "Layer.h"
#include "NNetwork.h"
#include "xmlparser.h"
#include "iomanager.h"

//Fonction qui affiche les valeurs d'un gsl_vector
void showGslVector(gsl_vector* gslVector)
{
	for(unsigned int i=0; i<gslVector->size; i++) {
		cout << "value " << i << " : " << gsl_vector_get(gslVector, i) << endl;
	}
}

/*Fonctions de test avec noms explicites pour tester
 * les fonctionnalités du programme
 */

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
	tlayer.calculOutput(preout,out,NULL);
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

void testForwardingVector()
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
	vector<double> output=nono.calculOutput(input);
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

void testForwardingMatrix()
{
	//Création et test du NNetwork
	NNetwork nono = xmlToNNetwork("testxml.xml");
	nono.printNetworkInfo();
	
	//Construction de l'input
	vector<vector<double>> input;
	vector<double> input1;
	vector<double> input2;
	input1.push_back(4);
	input1.push_back(5);
	input1.push_back(2);
	input2=input1;
	input.push_back(input1);
	input.push_back(input2);

	//Calcul et test de l'output
	vector<vector<double>> output=nono.calculOutput(input);
	for(unsigned int j=0; j<output.size(); j++) {
		cout << "OUTPUT" << endl;
		for(unsigned int k=0; k<output.at(j).size(); k++) {
			cout << output.at(j).at(k) << " ";
		}
		cout << endl;
	}
}

void testForwardingMatrixFromFile()
{
	//Création et test du NNetwork
	NNetwork nono = xmlToNNetwork("testxml.xml");
	nono.printNetworkInfo();
	
	//Construction de l'input
	vector<vector<double>> inputs(getInputsFromFile("testInputFile"));
	for(unsigned int j=0; j<inputs.size(); j++) {
		cout << "INPUT" << endl;
		for(unsigned int k=0; k<inputs.at(j).size(); k++) {
			cout << inputs.at(j).at(k) << " ";
		}
		cout << endl;
	}


	//Calcul et test de l'output
	vector<vector<double>> output=nono.calculOutput(inputs);
	for(unsigned int j=0; j<output.size(); j++) {
		cout << "OUTPUT" << endl;
		for(unsigned int k=0; k<output.at(j).size(); k++) {
			cout << output.at(j).at(k) << " ";
		}
		cout << endl;
	}
}

void testOutputInFile()
{
	//Création et test du NNetwork
	NNetwork nono = xmlToNNetwork("testxml.xml");
	nono.printNetworkInfo();
	
	//Construction de l'input
	vector<vector<double>> inputs(getInputsFromFile("testInputFile"));
	
	//Construction de l'output
	vector<vector<double>> outputs=nono.calculOutput(inputs);
	
	putOutputToFile("testOutputFile", outputs);
}

void testMNISTreader()
{
  vector<vector<double>> ar;
  ReadMNIST(10000,784,ar);
  cout << "test size :" << ar.at(0).size() << endl;
	for(unsigned int i=0; i<1; i++) {
		for(unsigned int j=0; j<ar.at(i).size(); j++) {
				printf("%f",ar.at(i).at(j));
				//cout << ar.at(i).at(j) << " ";				
		}
		cout << endl;;
	}
}

void testRandomizeGaussian()
{
	int nben(5), nbout(4);
	Layer layer(nben,nbout);
	layer.randomizeGaussian(1.0);
	layer.printLayerInfo();
}

void testGenAleaGaussianSigmoidNNetwork()
{
	vector<int> sizeLayers;
	sizeLayers.push_back(4);
	sizeLayers.push_back(2);
	sizeLayers.push_back(10);
	sizeLayers.push_back(150);
	sizeLayers.push_back(2);
	
	NNetwork nono(sizeLayers, 10.0);
	nono.printNetworkInfo();
}

void testMNISTNetwork()
{
	vector<int> sizeLayers;
	sizeLayers.push_back(784);
	sizeLayers.push_back(15);
	sizeLayers.push_back(10);
	
	NNetwork nono(sizeLayers, 1.0);
	
		
	vector<vector<double>> ar;
	ReadMNIST(10000,784,ar);
	vector<double> input;
	for(unsigned int i=0; i<1; i++) {
		for(unsigned int j=0; j<ar.at(i).size(); j++) {
				input.push_back(ar.at(i).at(j));				
		}
	}
	cout << "test 1 :" <<  input.size() << endl;
	//Calcul et test de l'output
	vector<double> output=nono.calculOutput(input);
	cout << "OUTPUT" << endl;
	for(unsigned int i=0; i<output.size(); i++) {
		cout << output.at(i) << endl;
	}
}

//Programme principal
int main(int argc, char* argv[]) {
	testMNISTNetwork();
	return 0;
}
