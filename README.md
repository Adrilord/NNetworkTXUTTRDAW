# NNetworkTXUTTRDAW
A good TX UTT project.
Le parser est pratiquement bon mais je dois remanier la classe Layer et NNetwork pour dissimuler l'utilisation de gsl (pour ne pas avoir des problèmes d'allocation et de désallocation de partout à cause des pointeurs) et je dois les fusionner pour optimiser le temps de calcul (et ne pas avoir à effectuer des tas de converstion entre les std::vector et les gsl_vector (et matrices))
