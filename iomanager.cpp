#include "iomanager.h"

vector<vector<double> > getInputsFromFile(string filename)
{
	vector<vector<double> > inputs;
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

void putOutputToFile(string filename, vector<vector<double> > outputs)
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

void ReadMNISTTrainingImages(int NumberOfImages, int DataOfAnImage,vector<vector<double> > &arr)
{
    arr.resize(NumberOfImages,vector<double>(DataOfAnImage));
    ifstream file("train-images.idx3-ubyte",ios::binary);
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

void ReadMNISTTrainingLabels(int NumberOfItems, vector<double> &arr)
{
    arr.resize(NumberOfItems,0);
    ifstream file("train-labels.idx1-ubyte",ios::binary);
    if (file.is_open())
    {
        int magic_number=0;
        int n_items=0;
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number= ReverseInt(magic_number);
        file.read((char*)&n_items,sizeof(n_items));
        n_items= ReverseInt(n_items);
        for(int i=0;i<n_items;++i)
        {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    arr[i]= (double)temp;
        }
    }
}

void extractingAMNISTMiniBatch(vector<vector<double> > &images, vector<double> &labels, 
	vector<vector<double> > &minibatchimages, vector<double> &minibatchlabels, unsigned int batchsize)
{	
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    for(unsigned int i=0; i<batchsize; i++) {
		std::uniform_int_distribution<> dis(1, images.size());
		int indice = dis(gen) - 1 ;
		//~ cout << "test indice " << indice << " test images.size() : " <<  images.size() << endl;
		//~ auto biggest = std::max_element(std::begin(images.at(indice)), std::end(images.at(indice)));
		//~ cout << "test max image value : " << *biggest << endl;
		minibatchimages.push_back(images.at(indice));
		minibatchlabels.push_back(labels.at(indice));
		images.erase (images.begin()+indice);
		labels.erase (labels.begin()+indice);
	}
}

void label2MNISTExpectedOutput(double &label, vector<double> &expectedOutput)
{
	for(int i=0; i<10; i++) {
		expectedOutput.push_back(0);
	}
	expectedOutput.at(label)=1.f;
}

void label2MNISTExpectedOutput(vector<double> &labels, vector<vector<double> > &expectedOutputs)
{
	for(unsigned int i=0; i<labels.size(); i++) {
		vector<double> expectedOutput;
		for(int j=0; j<10; j++) {
			expectedOutput.push_back(0);
		}
		expectedOutput.at(labels.at(i))=1.f;
		expectedOutputs.push_back(expectedOutput);
	}
}

void outputToLabel(double &label, vector<double> &output)
{
	label=0;
	for(unsigned int i=0; i<output.size(); i++) {
		if (output.at(i)>output.at(label)) {
			label = i;
		}
	}
}

void outputToLabels(vector<double> &labels, vector<vector<double> > &outputs)
{
	for(unsigned int i=0; i<outputs.size(); i++) {
		labels.push_back(0);
		for(unsigned int j=0; j<outputs.at(i).size(); j++) {
			if (outputs.at(i).at(j)>outputs.at(i).at(labels.at(i))) {
				labels.at(i) = j;
			}
		}	
	}
}
