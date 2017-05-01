/*Balises à spécifier (attributs) :

<NeuralNetwork>

<Layer>

typeid="0","1","2" //Input, Hidden, Output
nbneurons=”1”,”2”...

<Neuron>

functionid="0","1","2","3"... //pour bosser plus vite sur les comparaisons
functionparameters="4 3" //pour des fonctions à paramètres
bias=”[value]” (ou valeur aléatoire)
weights=”[value0] [value1] … [value[nbneurons]]” // (attention à avoir le bon nombre) (ou valeur aléatoire)
*/

#define INPUT 0
#define HIDDEN 1
#define OUTPUT 2
#define ID 0
#define SIGMOID 1
#define SIGMOIDP 2
#define TANH 3
#define GAUSSIAN 4 // Le premier paramètre correspond au beta de l'exp et les suivants aux coordonnées de centres du neurone
