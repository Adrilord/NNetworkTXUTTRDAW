#include "xmlparser.h"

NNetwork xmlToNNetwork(string xmlfilename) {
	//layers to fill
	vector<Layer> layers;

	//Ouverture du fichier xml et création d'un handle
	TiXmlDocument doc(xmlfilename);
	if(!doc.LoadFile()){
		cerr << "erreur lors du chargement" << endl;
		cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		return NNetwork(layers);
	}
	TiXmlHandle hdl(&doc);

	//On récupère le premier layer
	TiXmlElement *layerElem = hdl.FirstChildElement().FirstChildElement().Element();

	//vérif
	if(!layerElem) {
		cerr << "Il n'existe pas de Layers ou le fichier est mal formé" << endl;
	}

	//Remplissage du NNetwork
	while (layerElem) {
		layers.push_back(xmlToLayer(layerElem),nnTypeId);
	}

	//On renvoie le NNetwork construit
	return NNetwork(layers);	

}

Layer xmlToLayer(TiXmlElement* layerElement, int nben) {

		Layer(int& nben, int& nbout, gsl_matrix* weights, gsl_vector* bias, vector<int>& functionsID, vector<vector<double>>& functionsParam);

		int layerTypeID = atoi(layerElement->Attribute("typeid"));
		int nbout = atoi(layerElement->Attribute("nbneurons"));
		switch layerTypeID {
			case INPUT :
				nben=nbout; //optionnel mais secure
				return Layer(nben, nbout);
				break;
			default :
				TiXmlElement* neurelem;	
				neurelem = layerElement.FirstChildElement;
				break;




}

vector<int> decodeListOfInt(char* values) {
	char* value = strtok (values," "); //on lit la première valeur
	vector<int> finalValues;
	while (value != NULL)
	{
		finalValues.push_back(atoi(value));//on construit le vector
		value = strtok (NULL, " ");//on lit la valeur suivante
	}
	return finalValues;
}

void saveNetwork(NNetwork& network, string xmlfilename) {

}

char* codeListOfInt(vector<int>& listOfInt) {

}
