#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <vector>
#include "emulator.h"
#include "parse.h"

int main(int argc, char *argv[]) {
    int verbosity_level = 0, n_cosmologies;
	const int nPCA = 15; //15 principal components
	std::vector<double> nlc, kmodes;
	csmpars CSM; // this is just a container for easier parsing of cosmological parameters
	string outfilename;


	/* GET INPUT PARAMETERS */
	CSM = ee2_parser(argc, argv);
	n_cosmologies = CSM.Omega_b.size();
	std::cout << "There are " << n_cosmologies << " cosmologies specified in the input." << std::endl;

	/* Initialize EE session */
    //printf("Construction of EuclidEmulator instance...\n");
    EuclidEmulator ee2 = EuclidEmulator();
    std::cout << "EuclidEmulator2 >> Session started... " << std::endl;

	/* Define cosmology struct */
	//printf("Defining cosmology...\n");
	for (int cntr = 0; cntr < n_cosmologies; cntr++){
		/* Instatiate the Cosmology class */
		Cosmology cosmo = Cosmology(CSM.Omega_b[cntr], CSM.Omega_m[cntr], \
								    CSM.Sum_m_nu[cntr], CSM.n_s[cntr], CSM.h[cntr], \
									CSM.w_0[cntr], CSM.w_a[cntr], CSM.A_s[cntr]);

		/* compute NLCs for each cosmology */
		ee2.compute_nlc(cosmo, CSM.zvec[cntr], CSM.n_redshift[cntr]);

		/* Write result to file. Format: k [h/Mpc] B(k,z0) B(k,z1) ... B(k,zn) */
		string filename = "my_nlc"+to_string(cntr)+".dat";
		ee2.write_nlc2file(filename, CSM.zvec[cntr], CSM.n_redshift[cntr]);
	}
	std::cout << "EuclidEmulator2 >> Session closed... " << std::endl;
	return 0;
}
