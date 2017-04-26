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
	
	int nben=0;
	int layerTypeID=0;
	int nbout=0;
	
	layerElem->QueryIntAttribute("nbneurons", &nben); // on détermine le nben à partir du layer précédent

	//Remplissage du NNetwork
	while (layerElem) {
		//Onrécupère les informations du layer
		layerElem->QueryIntAttribute("nbneurons", &nbout);
		layerElem->QueryIntAttribute("typeid", &layerTypeID);
		
		//On analyse les neurones à ce niveau, ici on récupère le premier
		TiXmlElement *neurElem = layerElem->FirstChildElement();
		//vérif
		if(!neurElem) {
			cerr << "Il n'existe pas de neurones ou le fichier est mal formé" << endl;
			return NNetwork(layers);
		}
		
		Layer *layerToPush = xmlToLayer(neurElem, nben, layerTypeID, nbout);
		cout << "layerToPush : " << &layerToPush << endl;
		layerToPush->printLayerInfo();
		layers.push_back(*layerToPush);
		cout << "PUSH" << endl;
		layerElem->QueryIntAttribute("nbneurons", &nben); // on détermine le nben à partir du layer précédent
		layerElem = layerElem->NextSiblingElement(); // iteration
	}
	//On renvoie le NNetwork construit
	return NNetwork(layers);	

}


Layer* xmlToLayer(TiXmlElement* neurElem, int nben, int layerTypeID, int nbout) {
	Layer* layerToConstruct = NULL;
	if(layerTypeID==INPUT) {
		nben=nbout; //optionnel mais secure
		layerToConstruct = new Layer(nben, nbout);
		return layerToConstruct;
	} else {
		//valeurs à lire
		vector<double> bias;
		vector<vector<double>> weights;
		vector<int> functionsID;
		vector<vector<double>> functionsParam;

		while(neurElem) {
			//décodages simples
			bias.push_back(atof(neurElem->Attribute("bias")));
			functionsID.push_back(atoi(neurElem->Attribute("functionid")));
			//décodages complexes (listes)
			string tempString;
			neurElem->QueryStringAttribute("weights",&tempString);
			weights.push_back(decodeListOfDouble(tempString));
			if (neurElem->QueryStringAttribute("functionparameters",&tempString)) {
				functionsParam.push_back(decodeListOfDouble(tempString));
			}
					
			neurElem = neurElem->NextSiblingElement(); // iteration 
		}
		
		layerToConstruct = new Layer(nben, nbout, weights, bias, functionsID, functionsParam);
		return layerToConstruct;
	}
}

//CA MARCHE
vector<double> decodeListOfDouble(string values) {

	vector<double> finalValues;
	std::istringstream s(values);
	double d;
	while (s >> d) {
		finalValues.push_back(d);
	}

    return finalValues;
}

void saveNetwork(NNetwork& network, string xmlfilename) {

}

char* codeListOfDouble(vector<double>& listOfDouble) {

	return NULL;
}
