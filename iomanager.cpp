#include "iomanager.h"

vector<vector<double>> getInputsFromFile(string filename)
{
	vector<vector<double>> inputs;
	ifstream fichier(filename);
	if (!fichier.bad()) { //permet de tester si le fichier s'est ouvert sans probleme
		string lign;
		while(getline(fichier, lign)) {
			//Chaque ligne correspond à un input
			//les lignes deviendront ensuite des vecteurs colonnes
			//de matrice
			inputs.push_back(decodeListOfDouble(lign));
		}
		fichier.close();
	} else {
		cerr << "Erreur ouverture fichier " << filename << endl;
	}
	return inputs;
}

void putOutputToFile(string filename, vector<vector<double>> outputs)
{
	ofstream fichier;
	fichier.open(filename, ios::out | ios::trunc); //on ouvre le fichier en ecriture
	if (fichier) { //permet de tester si le fichier s'est ouvert sans probleme
		for(unsigned int j=0; j < outputs.size(); j++) {
			for(unsigned int k=0; k < outputs.at(j).size(); k++) {
				fichier <<  outputs.at(j).at(k) << " ";
			}
			fichier << endl;
		}
		fichier.close();
	} else {
		cerr << "Erreur ouverture fichier " << filename << endl;
	}
}

int ReverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}

void ReadMNIST(int NumberOfImages, int DataOfAnImage,vector<vector<double>> &arr)
{
    arr.resize(NumberOfImages,vector<double>(DataOfAnImage));
    ifstream file("t10k-images.idx3-ubyte",ios::binary);
    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number= ReverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= ReverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= ReverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= ReverseInt(n_cols);
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    arr[i][(n_rows*r)+c]= (double)temp;
                }
            }
        }
    }
}
