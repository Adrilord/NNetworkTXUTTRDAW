#include "Layer.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_exp.h>
#include <cstdio>

int main(int argc, char* argv[]) {
	int nben=10;
	int nbout=10;
	Layer lol = Layer(nben,nbout);
	gsl_vector* en = gsl_vector_alloc(nben);
	for(int i=0; i<nben; i++) {
		gsl_vector_set (en, i, 5);
	}
	gsl_vector* testout = gsl_vector_alloc(nbout);
	testout = lol.calculPreOutput(en);
	testout = lol.calculOuput(testout);
	for(int i=0; i<nbout; i++) {
		printf ("vout_%d = %g\n", i, gsl_vector_get (testout, i));
	}
	
	return 0;
}
