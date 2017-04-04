#include "xmlparser.h"

xmlparser::xmlparser(string xmlfile) {
	//Ouverture du fichier xml
	TiXmlDocument doc(xmlfile);
	if(!doc.LoadFile()){
		cerr << "erreur lors du chargement" << endl;
		cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		return 1;
	} else {
		this->docNetwork = doc;
	}
}

NNetwork readNNetwork() {
	//Création d'un handle et d'une variable element pour parcourir les données
	TiXmlHandle hdl(&doc);
	TiXmlElement *elem = hdl.FirstChildElement().Element();
}

saveNetwork(string xmlfilename) {
	
}
