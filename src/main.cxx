#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <typeinfo>

#include "emulator.h"

int main(int argc, char *argv[]) {
	const int nPCA = 15; //15 principal components
	double * nlc;

    double * Omega_b;
    double * Omega_m;
    double * Sum_m_nu;
    double * n_s;
    double * h;
    double * w_0;
    double * w_a;
    double * A_s;

	int n_redshift = 0, n_cosmologies = 0;
	double ** zvec;
	double * kmodes;

	FILE * cosmofile;
    char instring[256];
	char * token;

	//for(int i = 0; i < 20; i++) zvec[0][i] = -1.0; 

	/* GET COSMOLOGICAL PARAMETERS FROM STDIN OR FILE */
	if (argc >= 10){
		printf("Reading from stdin...\n");
		Omega_b = new double[1];
		Omega_m = new double[1];
		Sum_m_nu = new double[1];
		n_s = new double[1];
		h = new double[1];
		w_0 = new double[1];
		w_a = new double[1];
		A_s = new double[1];
		zvec = new double*[1];
        zvec[0] = new double[50];

		Omega_b[0] = atof(argv[1]);
		Omega_m[0] = atof(argv[2]);
		Sum_m_nu[0] = atof(argv[3]);
		n_s[0] = atof(argv[4]);
		h[0] = atof(argv[5]);
		w_0[0] = atof(argv[6]);
		w_a[0] = atof(argv[7]);
		A_s[0] = atof(argv[8]);

		n_cosmologies = 1;

		n_redshift = argc - 9;
		//printf("Computing the NLC at %d different redshifts:\n", n_redshift);
		string zvecstr = "zvec = ["; 
		for(int i=9; i<argc; i++){
			zvec[0][i-9] = atof(argv[i]);
			zvecstr.append(to_string(zvec[0][i-9]));
			if (i != argc-1) {
				zvecstr.append(", ");
			}
		}
		zvecstr.append("]");
		cout << zvecstr << endl;
	}
	else if(argc==2){
		cosmofile = fopen(argv[1], "r");
		if(cosmofile == NULL){
			std::cout << "Could not open cosmology file " <<  argv[1] << std::endl;
			std::cout << "Please make sure the command line argument list follows the format: " << std::endl;
			std::cout << std::endl;
			std::cout << "  ./ee2.exe [/path/to/cosmology/file]" << std::endl;
			std::cout << "or" << std::endl;
			std::cout << "  ./ee2.exe [Omega_b] [Omega_m] [Sum m_nu] [n_s] [h] [w_0] [w_a] [A_s] [z_1] ... [z_n]" << std::endl;
			exit(1);
		}
		
		std::cout << "Reading from file..." << endl;
		
		//Count number of cosmologies specified in input file:	
		while (fgets(instring, 256, cosmofile) != NULL){
			// Don't count the line if it's a comment (starts with a #) or if it's empty
			if (instring[0] == '#'){
                continue;
            }
			++n_cosmologies;
		}
		fclose(cosmofile);
		std::cout << "There are " << n_cosmologies << " cosmologies specified in the input file" << endl;	

		// Reallocate arrays for cosmological parameters
		Omega_b = new double[n_cosmologies];
		Omega_m = new double[n_cosmologies];
		Sum_m_nu = new double[n_cosmologies];
		n_s = new double[n_cosmologies];
		h = new double[n_cosmologies];
		w_0 = new double[n_cosmologies];
		w_a = new double[n_cosmologies];
		A_s = new double[n_cosmologies];
		zvec = new double*[n_cosmologies];
		for(int i = 0; i < n_cosmologies; ++i){
			zvec[i] = new double[50];
		}

		// Fill arrays
		int linecntr = 0;
		n_redshift = 1;
		cosmofile = fopen(argv[1], "r");
		bool read_next_line = true;
		while(read_next_line){	
			if (fgets(instring, 256, cosmofile) != NULL){	
				token = strtok(instring, ",");
				Omega_b[linecntr] = atof(token);

				token = strtok(NULL, ",");
				Omega_m[linecntr] = atof(token);

                token = strtok(NULL, ",");
				Sum_m_nu[linecntr] = atof(token);

                token = strtok(NULL, ",");
				n_s[linecntr] = atof(token);

                token = strtok(NULL, ",");
				h[linecntr] = atof(token);

                token = strtok(NULL, ",");
				w_0[linecntr] = atof(token);

                token = strtok(NULL, ",");
				w_a[linecntr] = atof(token);

                token = strtok(NULL, ",");
				A_s[linecntr] = atof(token);

                token = strtok(NULL, ",");
				zvec[linecntr][0] = atof(token);
				linecntr++;
			}
			else{
				read_next_line = false;
			}	
		}
		fclose(cosmofile);
	}
	else{
		std::cout << "Invalid command line inputs" << std::endl;
		exit(1);
	}

	/* Initialize EE session */
    //printf("Construction of EuclidEmulator instance...\n");
    EuclidEmulator ee2 = EuclidEmulator();
    std::cout << "EuclidEmulator2 >> Session started... " << std::endl;

	/* Define cosmology struct */
	//printf("Defining cosmology...\n");
	for (int cntr = 0; cntr < n_cosmologies; cntr++){
		Cosmology cosmo = Cosmology(Omega_b[cntr], Omega_m[cntr], Sum_m_nu[cntr], n_s[cntr], h[cntr], w_0[cntr], w_a[cntr], A_s[cntr]);

		/* compute NLCs for each cosmology */
		ee2.compute_nlc(cosmo, zvec[cntr], n_redshift);

		/* Write result to file. Format: k [h/Mpc] B(k,z0) B(k,z1) ... B(k,zn) */
		string filename = "my_nlc"+to_string(cntr)+".dat";
		ee2.write_nlc2file(filename, zvec[cntr], n_redshift);
	}
	std::cout << "EuclidEmulator2 >> Session closed... " << std::endl;
	return 0;
}
