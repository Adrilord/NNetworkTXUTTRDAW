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
		layerElem->QueryIntAttribute("nbneurons", &nbout);
		layerElem->QueryIntAttribute("typeid", &layerTypeID);
		//On récupère le premier neurone
		TiXmlElement *neurElem = layerElem->FirstChildElement();
		layers.push_back(xmlToLayer(neurElem, nben, layerTypeID, nbout));
		layerElem->QueryIntAttribute("nbneurons", &nben); // on détermine le nben à partir du layer précédent
		layerElem = layerElem->NextSiblingElement(); // iteration
	}

	//On renvoie le NNetwork construit
	return NNetwork(layers);	

}


Layer xmlToLayer(TiXmlElement* neurElem, int& nben, int& layerTypeID, int& nbout) {
		cout << "nben : " << nben << " nbout : " << nbout << endl;
		switch (layerTypeID) {
			case INPUT :
				nben=nbout; //optionnel mais secure
				return Layer(nben, nbout);
				break;
			default :
				//vérif
				if(!neurElem) {
					cerr << "Il n'existe pas de neurones ou le fichier est mal formé" << endl;
					return Layer(nben, nbout);
				}

				//valeurs à lire
				vector<double> tempbias;
				vector<vector<double>> tempweights;
				vector<int> functionsID;
				vector<vector<double>> functionsParam;

				while(neurElem) {
					//décodages simples
					tempbias.push_back(atof(neurElem->Attribute("bias")));
					functionsID.push_back(atoi(neurElem->Attribute("functionid")));
					//décodages complexes (listes)
					string tempString;
					neurElem->QueryStringAttribute("weights",&tempString);
					tempweights.push_back(decodeListOfDouble(tempString));
					if (neurElem->QueryStringAttribute("functionparameters",&tempString)) {
						functionsParam.push_back(decodeListOfDouble(tempString));
					}
					
					neurElem = neurElem->NextSiblingElement(); // iteration 
				}
				//~ cout << "bonjour nbout : " << nbout << " nben : " << nben << " layerTypeID : " << layerTypeID << endl;
				//~ cout << "bonjour sizetempbias : " << tempbias.size() << endl ;
				gsl_vector* bias=gsl_vector_alloc (nbout);
				for(int i=0; i<nbout; i++) {
					gsl_vector_set (bias, i, tempbias.at(i));
				}
				//~ cout << "bonjour sizeTempWeights 1 :" << tempweights.size() << " 2 : " << tempweights.at(0).size() << endl;
				gsl_matrix* weights = gsl_matrix_alloc (nbout, nben);
				for(int j=0; j<nbout; j++) {
					for(int k=0; k<nben; k++) {
							gsl_matrix_set (weights, j, k, tempweights.at(j).at(k));
					}
				}
				
				cout << "YOLO" << endl;

				return Layer(nben, nbout, weights, bias, functionsID, functionsParam);

				break;
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
