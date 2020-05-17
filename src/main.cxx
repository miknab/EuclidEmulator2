#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "emulator.h"

int main(int argc, char *argv[]) {
	const int nPCA = 15; //15 principal components
	double* nlc;

    double Omega_b = 0.0;
    double Omega_m = 0.0;
    double Sum_m_nu = 0.0;
    double n_s = 0.0;
    double h = 0.0;
    double w_0 = 0.0;
    double w_a = 0.0;
    double A_s = 0.0;
	int n_redshift = 0;
	double zvec[20];
	double *kmodes;

	for(int i = 0; i < 20; i++) zvec[i] = -1.0; 

	/* GET COSMOLOGICAL PARAMETERS FROM STDIN OR FILE */
	if (argc >= 10){
		printf("Reading from stdin...\n");
		Omega_b = atof(argv[1]);
		Omega_m = atof(argv[2]);
		Sum_m_nu = atof(argv[3]);
		n_s = atof(argv[4]);
		h = atof(argv[5]);
		w_0 = atof(argv[6]);
		w_a = atof(argv[7]);
		A_s = atof(argv[8]);

		n_redshift = argc - 9;
		printf("Computing the NLC at %d different redshifts:\n", n_redshift);
		string zvecstr = "zvec = ["; 
		for(int i=9; i<argc; i++){
			zvec[i-9] = atof(argv[i]);
			zvecstr.append(to_string(zvec[i-9]));
			if (i != argc-1) {
				zvecstr.append(", ");
			}
		}
		zvecstr.append("]");
		cout << zvecstr << endl;
	}
	else if(argc==1){
		ifstream cosmofile(argv[1]);

		if(!cosmofile){
			std::cout << "Could not open cosmology file " <<  argv[1] << std::endl;
			std::cout << "Please make sure the command line argument list follows the format: " << std::endl;
			std::cout << std::endl;
			std::cout << "  ./ee2.exe [/path/to/cosmology/file]" << std::endl;
			std::cout << "or" << std::endl;
			std::cout << "  ./ee2.exe [Omega_b] [Omega_m] [Sum m_nu] [n_s] [h] [w_0] [w_a] [A_s] [z_1] ... [z_n]" << std::endl;
		}
	}
	else{
		std::cout << "Invalid command line inputs" << std::endl;
		exit(1);
	}

	/* Define cosmology struct */
	printf("Defining cosmology...\n");
	Cosmology cosmo = Cosmology(Omega_b, Omega_m, Sum_m_nu, n_s, h, w_0, w_a, A_s);


	/* Initialize EE session */
	printf("Construction of EuclidEmulator instance...\n");
	EuclidEmulator ee2 = EuclidEmulator();
	std::cout << "EuclidEmulator2 >> Session started... " << std::endl;

	/* compute NLCs for each cosmology */
	ee2.compute_nlc(cosmo, zvec, n_redshift);
	//ee2.compute_nlc(cosmo, zvec, n_redshift, kmodes, sizeof(kmodes)/sizeof(double));
	printf("NLC computation complete\n");

	/* Write result to file. Format: k [h/Mpc] B(k,z0) B(k,z1) ... B(k,zn) */
	string filename = "my_nlc.dat";
	ee2.write_nlc2file(filename, zvec, n_redshift);

	std::cout << "EuclidEmulator2 >> Session closed... " << std::endl;
	return 0;
}
