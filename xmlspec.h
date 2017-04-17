/*Balises à spécifier (attributs) :

<NeuralNetwork>

<Layer>

typeid="0","1","2" //Input, Hidden, Output
nbneurons=”1”,”2”...

<Neuron>

function="id",”sigmoïd”,”door”,... // (si le type de NNetwork est paramétrable)
functionid="0","1","2","3"... //pour bosser plus vite sur les comparaisons
functionparameters="4 3" //pour des fonctions à paramètres
bias=”[value]” (ou valeur aléatoire)
weights=”[value0] [value1] … [value[nbneurons]]” // (attention à avoir le bon nombre) (ou valeur aléatoire)
*/

#define INPUT 0
#define HIDDEN 1
#define OUTPUT 2
#define PARAM 0
#define ID 0
#define SIGMOID 1
