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
	FILE * cosmofile;
    char instring[256];
	char * token;
	string outfilename;


	/* GET INPUT PARAMETERS */
	CSM = ee2_parser(argc, argv);
	n_cosmologies = CSM.Omega_b.size();
	/* GET COSMOLOGICAL PARAMETERS FROM STDIN OR FILE */
	/*
	if(argc==2){
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

		// Fill arrays
		int linecntr = 0;
		cosmofile = fopen(argv[1], "r");
		bool read_next_line = true;
		while(read_next_line){	
			if (fgets(instring, 256, cosmofile) != NULL){	
				//Remark: Don't forget to split (i.e. tokenize) each
				//        line into its individual parameters
				token = strtok(instring, ",");
				Omega_b.push_back(atof(token));

				token = strtok(NULL, ",");
				Omega_m.push_back(atof(token));

                token = strtok(NULL, ",");
				Sum_m_nu.push_back(atof(token));

                token = strtok(NULL, ",");
				n_s.push_back(atof(token));

                token = strtok(NULL, ",");
				h.push_back(atof(token));

                token = strtok(NULL, ",");
				w_0.push_back(atof(token));

                token = strtok(NULL, ",");
				w_a.push_back(atof(token));

                token = strtok(NULL, ",");
				A_s.push_back(atof(token));

				// Read the rest of the line as individual redshifts:
				std::vector<double> z_tmp;
				bool reached_end_of_line = false;
				while (!reached_end_of_line){	
                	token = strtok(NULL, ",");
					printf("Redshifts: %s\n", token);
					z_tmp.push_back(atof(token));
					if (strstr(token, "\n") != NULL) reached_end_of_line = true;
				}
				zvec.push_back(z_tmp);
				n_redshift.push_back(z_tmp.size()); 
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
	*/
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
