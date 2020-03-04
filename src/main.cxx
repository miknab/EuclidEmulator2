#include <iostream>
#include <string>

#include "emulator.h"
#include "cosmology.h"

int main(int argc, char *argv[]) {
	const int nPCA = 15; //15 principal components
	double* nlc;
	/* Initialize EE session */
	EuclidEmulator ee2 = EuclidEmulator();

	/* Read-in cosmology/cosmologies */
	Cosmology mycsm = Cosmology();

	/* compute NLCs for each cosmology */
	int csm_cntr = 0;
	ee2.compute_nlc(mycsm, nlc);
	return 0;
}
