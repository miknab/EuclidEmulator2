#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <vector>

#include "emulator.h"

using namespace std; // contains std::vector, std::cout, std::cin etc

int main(int argc, char *argv[]) {
	const int nPCA = 15; //15 principal components
	vector<double> nlc;
	vector<double> Omega_b;
    vector<double> Omega_m;
    vector<double> Sum_m_nu;
    vector<double> n_s;
    vector<double> h;
    vector<double> w_0;
    vector<double> w_a;
    vector<double> A_s;
	vector<int> n_redshift;
	int n_cosmologies = 0;
	vector< vector<double> > zvec;
	vector<double> kmodes;

	FILE * cosmofile;
    char instring[256];
	char * token;

	//for(int i = 0; i < 20; i++) zvec[0][i] = -1.0; 

	/* GET COSMOLOGICAL PARAMETERS FROM STDIN OR FILE */
	if (argc >= 10){
		printf("Reading from stdin...\n");
		// We can only read one single cosmology from the command line
		n_cosmologies = 1;
		// There my be many redshift values though
		n_redshift.push_back(argc - 9);
		// Store the cosmological parameters into the respective variables
		// --> the variables will now be double-vectors of length 1
		Omega_b.push_back(atof(argv[1]));
		Omega_m.push_back(atof(argv[2]));
		Sum_m_nu.push_back(atof(argv[3]));
		n_s.push_back(atof(argv[4]));
		h.push_back(atof(argv[5]));
		w_0.push_back(atof(argv[6]));
		w_a.push_back(atof(argv[7]));
		A_s.push_back(atof(argv[8]));

		printf("Computing the NLC at %d different redshifts:\n", n_redshift.at(0));
		string zvecstr = "zvec = ["; 
		vector<double> z_tmp;
		for(int i=9; i<argc; i++){
			z_tmp.push_back(atof(argv[i]));
			zvecstr.append(argv[i]);
			if (i != argc-1) {
				zvecstr.append(", ");
			}
		}
		zvec.push_back(z_tmp);
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
				vector<double> z_tmp;
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

	/* Initialize EE session */
    //printf("Construction of EuclidEmulator instance...\n");
    EuclidEmulator ee2 = EuclidEmulator();
    std::cout << "EuclidEmulator2 >> Session started... " << std::endl;

	/* Define cosmology struct */
	//printf("Defining cosmology...\n");
	for (int cntr = 0; cntr < n_cosmologies; cntr++){
		Cosmology cosmo = Cosmology(Omega_b[cntr], Omega_m[cntr], Sum_m_nu[cntr], n_s[cntr], h[cntr], w_0[cntr], w_a[cntr], A_s[cntr]);

		/* compute NLCs for each cosmology */
		ee2.compute_nlc(cosmo, zvec[cntr], n_redshift[cntr]);

		/* Write result to file. Format: k [h/Mpc] B(k,z0) B(k,z1) ... B(k,zn) */
		string filename = "my_nlc"+to_string(cntr)+".dat";
		ee2.write_nlc2file(filename, zvec[cntr], n_redshift[cntr]);
	}
	std::cout << "EuclidEmulator2 >> Session closed... " << std::endl;
	return 0;
}
