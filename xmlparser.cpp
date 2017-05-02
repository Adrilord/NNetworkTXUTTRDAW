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
		//On récupère les informations du layer
		layerElem->QueryIntAttribute("nbneurons", &nbout);
		layerElem->QueryIntAttribute("typeid", &layerTypeID);
		
		//On analyse les neurones à ce niveau, ici on récupère le premier
		TiXmlElement *neurElem = layerElem->FirstChildElement();
		//vérif
		if(!neurElem) {
			cerr << "Il n'existe pas de neurones ou le fichier est mal formé" << endl;
			return NNetwork(layers);
		}
		
		Layer layerToPush = xmlToLayer(neurElem, nben, layerTypeID, nbout);
		layers.push_back(layerToPush);
		layerElem->QueryIntAttribute("nbneurons", &nben); // on détermine le nben à partir du layer précédent
		layerElem = layerElem->NextSiblingElement(); // iteration
	}
	return NNetwork(layers);	
}


Layer xmlToLayer(TiXmlElement* neurElem, int nben, int layerTypeID, int nbout) 
{
	if(layerTypeID==INPUT) {
		nben=nbout; //optionnel mais secure
		return Layer(nben, nbout);
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
			
			//décodages poids
			string tempStringWeights;
			neurElem->QueryStringAttribute("weights",&tempStringWeights);
			weights.push_back(decodeListOfDouble(tempStringWeights));
			
			//décodages paramètres de fonction
			string tempStringParams;
			neurElem->QueryStringAttribute("functionparameters",&tempStringParams);
			if(!tempStringParams.empty()) {
				functionsParam.push_back(decodeListOfDouble(tempStringParams));
			} else {
				vector<double> zero;
				zero.push_back(0);
				functionsParam.push_back(zero);
			}
					
			neurElem = neurElem->NextSiblingElement(); // iteration 
		}
		
		return Layer(nben, nbout, weights, bias, functionsID, functionsParam);
	}
}

vector<double> decodeListOfDouble(string values) 
{
	vector<double> finalValues;
	std::istringstream s(values);
	double d;
	while (s >> d) {
		finalValues.push_back(d);
	}

    return finalValues;
}

void saveNetwork(NNetwork& network, string xmlfilename) 
{
	//Création du fichier s'il n'existe pas (!!ne supprime rien!!)
	//Mise en place des balises initiales
	ofstream objetfichier;
	objetfichier.open(xmlfilename, ios::out); //on ouvre le fichier en ecriture
	if (!objetfichier.bad()) { //permet de tester si le fichier s'est ouvert sans probleme
		objetfichier << "<?xml version='1.0' encoding='UTF-8' ?>" << endl;
		objetfichier << "<NNetwork>" << endl;
		objetfichier << "</NNetwork>" << endl;
		objetfichier.close(); //on ferme le fichier pour liberer la mémoire
	} else {
		cerr << "Erreur création fichier " << xmlfilename << endl;
	}
		
	//Ouverture du fichier xml (et création d'un handle)
	TiXmlDocument doc(xmlfilename);
	if(!doc.LoadFile()){
		cerr << "erreur lors du chargement" << endl;
		cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
	}
	//TiXmlHandle hdl(&doc);
	TiXmlElement *nnElem = doc.FirstChildElement();
	vector<Layer> layers = network.getLayers();
	for(unsigned int l=0; l<layers.size(); l++) {
		TiXmlElement newLayer("Layer");
		//On détecte le type de Layer
		if(l==0) {
			newLayer.SetAttribute("typeid", "0");
		} else if (l==layers.size()-1) {
			newLayer.SetAttribute("typeid", "2");
		} else {
			newLayer.SetAttribute("typeid", "1");
		}
		//On indique le nombre de neurones
		newLayer.SetAttribute("nbneurons", layers.at(l).getNbOut());
		for(int n=0; n<layers.at(l).getNbOut(); n++) {
			TiXmlElement newNeuron("Neuron");
			//On génère tous les attributs pour chaque neurone
			newNeuron.SetAttribute("functionid",layers.at(l).getFunctionsID().at(n));
			if(layers.at(l).getFunctionsParam().size() >= 1 ) {
				newNeuron.SetAttribute("functionparameters",codeListOfDouble(layers.at(l).getFunctionsParam().at(n)));	
			}
			newNeuron.SetAttribute("bias",layers.at(l).getBias().at(n));
			newNeuron.SetAttribute("weights",codeListOfDouble(layers.at(l).getWeights().at(n)));
			newLayer.InsertEndChild(newNeuron);
		}
		nnElem->InsertEndChild(newLayer);
	}
	doc.SaveFile(xmlfilename);  // enregistrement des modifications
}

char* codeListOfDouble(vector<double>& listOfDouble) 
{
	ostringstream stringList;
	for(unsigned int i=0; i<listOfDouble.size(); i++) {
		stringList << listOfDouble.at(i) << " ";
	}
	string str = stringList.str();
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}
