#include <parse.h>
#include <assert.h>


csmpars ee2_parser(int n_args, char * vec_args[]){
	size_t n_cosmo_pars;
	csmpars CSM;
	// Option 1: pass in a cosmology following the format:
    //      Omega_b, Omega_m, Sum_m_nu, n_s, h, w_0, w_a, A_s, z[0], ..., z[n]
        // yet to be added

    // Option 2: pass in a cosmology, a camb, a class or a parameter file
    // define the Options class and add the individual options to it
    cxxopts::Options options("EuclidEmulator2", "Highly accurate and efficient AI-based \
predictor of the non-linear correction of the matter power spectrum.");
    options.add_options()
        ("b,Omega_b", "baryon density parameter", cxxopts::value<double>())
        ("m,Omega_m", "(total) matter density parameter", cxxopts::value<double >())
        ("s,Sum_m_nu", "sum of neutrino masses in eV", cxxopts::value<double>())
        ("n,n_s", "spectral index", cxxopts::value<double>())
        ("H,hubble", "dimensionless Hubble parameter", cxxopts::value<double>())
        ("W,w_0", "time-independent dark energy equation-of-state parameter", cxxopts::value<double>())
        ("w,w_a", "time-dependent dark energy equation-of-state parameter", cxxopts::value<double>())
        ("A,A_s", "spectral amplitude", cxxopts::value<double>())
        ("z,redshift", "redshift at which the NLC shall be computed", cxxopts::value< std::vector< double > >())
        ("l,classfile", "path to CLASS parameter file", cxxopts::value<std::string>())
        ("a,cambfile", "path to CAMB parameter file", cxxopts::value<std::string>())
        ("p,parfile", "path to EE2 parameter file", cxxopts::value<std::string>())
        ("o,outfile", "output file name", cxxopts::value<std::string>()->default_value("nlc"))
		("d,outdir", "output directory", cxxopts::value<std::string>()->default_value("results"))
        ("v,verbosity", "verbosity level (0, 1 or 2)", cxxopts::value<int>()->default_value("0"))
        ("h,help", "Print usage");

	// Now we actually start parsing in the arguments...
	auto result = options.parse(n_args, vec_args);
	
	if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }
	CSM.verbosity_level = result["verbosity"].as<int>();

    if (result.count("outfile")){
        CSM.outfilename = result["outfile"].as<std::string>();
    }

	// n_cosmo_pars counts the number of flags defining cosmological parameters
	// on the commmand line
	n_cosmo_pars = result.count("Omega_b") + result.count("Omega_m") \
				 + result.count("Sum_m_nu") + result.count("n_s") \
				 + result.count("hubble") + result.count("w_0") \
				 + result.count("w_a") + result.count("A_s");

	// If the cosmology shall be read from a file, then no cosmological parameter
	// may be defined on the command line
	if (n_cosmo_pars > 0){
		if (n_cosmo_pars < 8){
			printf("Cosmology not fully defined!\n");
			exit(1);
		}
		read_cosmo_from_cmdline(result, CSM);
	}
	if (result.count("classfile")){
		assert(n_cosmo_pars == 0);
       	read_classfile(result["classfile"].as<std::string>(), CSM);
	   }
    if (result.count("cambfile")){
		assert(n_cosmo_pars == 0);
       	read_cambfile(result["cambfile"].as<std::string>(), CSM);
    }
    if (result.count("parfile")){
		assert(n_cosmo_pars == 0);
       	read_parfile(result["parfile"].as<std::string>(), CSM);
   	}
	
	return CSM;
}

void read_cosmo_from_cmdline(cxxopts::ParseResult result, csmpars &CSM){
	std::cout << "Reading cosmology from command line..." << std::endl;

	if (result.count("Omega_b")){
		CSM.Omega_b.push_back(result["Omega_b"].as<double>());
    }
    if (result.count("Omega_m")){
        CSM.Omega_m.push_back(result["Omega_m"].as<double>());
    }
    if (result.count("Sum_m_nu")){
        CSM.Sum_m_nu.push_back(result["Sum_m_nu"].as<double>());
    }
    if (result.count("n_s")){
        CSM.n_s.push_back(result["n_s"].as<double>());
    }
    if (result.count("hubble")){
        CSM.h.push_back(result["hubble"].as<double>());
    }
    if (result.count("w_0")){
        CSM.w_0.push_back(result["w_0"].as<double>());
    }
	if (result.count("w_a")){
        CSM.w_a.push_back(result["w_a"].as<double>());
    }
    if (result.count("A_s")){
        CSM.A_s.push_back(result["A_s"].as<double>());
    }
    if (result.count("redshift")){
		std::string zvecstr = "zvec = [";
		CSM.n_redshift.push_back(result.count("redshift"));
		std::vector<double> ztmp = result["redshift"].as< std::vector<double> >();
        CSM.zvec.push_back(ztmp); 
        for(int i=0; i<CSM.n_redshift.at(0); i++){
            zvecstr.append(std::to_string(CSM.zvec.at(0).at(i)));
            if (i != CSM.n_redshift.at(0)-1) {
                zvecstr.append(", ");
            }
        }
        zvecstr.append("]");
        std::cout << zvecstr << std::endl;
    }
}

bool starts_with(std::string str1, std::string str2){
/* Checks if str1 starts with str2 (modulo white spaces). */
	return str1.rfind(str2, 0) == 0;
}

double extract_value(std::string line){
/* Expects lines of the format '[some characters] = [number]  *\
|* This function extracts the [number] pattern and returns it *|
\* as a double.												  */

	double value;

	// Split line at "=" and access substring after "=":
	std::string value_str = line.substr(line.find("=")+1);

	// Convert value_str to double
	value = atof(value_str.c_str());

	return value;	
}

void read_classfile(std::string class_file_name, csmpars &CSM){
    printf("Reading cosmology from CLASS file...\n");
	FILE * classfile;
    char instring[256];
	std::string current_line;
    char * token;

    // Open parameter file (and check for success):
    classfile = fopen(class_file_name.c_str(), "r");
    if(classfile == NULL){
        std::cout << "Could not open CLASS file " <<  class_file_name << std::endl;
        exit(1);
	}

	// Read file line by line
	double Omega_cdm;
	bool read_next_line = true;
	bool hubble_defined = false;
	bool little_omegab_defined = false;
	bool little_omegacdm_defined = false;
	bool mnu_defined = false;
	bool Omega_ncdm_defined = false;
	bool As_defined = false;
    while(read_next_line){
        if (fgets(instring, 256, classfile) != NULL){
			current_line = instring;
			// Hubble parameter:
			if (starts_with(current_line, "H0 =") || starts_with(current_line, "H0=")){
				CSM.h.push_back(0.01*extract_value(current_line));
				hubble_defined = true;
			}
			if (starts_with(current_line, "h =") || starts_with(current_line, "h=")){
				CSM.h.push_back(extract_value(current_line));
				hubble_defined = true;
			}
			// Baryon density parameter: 
			if (starts_with(current_line, "Omega_b =") || starts_with(current_line, "Omega_b=")){
				if(little_omegacdm_defined){
					std::cout << "\n==> ERROR <==: Please don't mix definitions of capital and lower-case Omega's." << std::endl;
					exit(1);
				}
				CSM.Omega_b.push_back(extract_value(current_line));
            }
			if (starts_with(current_line, "omega_b =") || starts_with(current_line, "omega_b=")){
				//if(!little_omegacdm_defined){
                //    std::cout << "\n==> ERROR <==: Please don't mix definitions of capital and lower-case Omega's." << std::endl;
                //    exit(1);
                //}
				CSM.Omega_b.push_back(extract_value(current_line));
				little_omegab_defined=true;
			}
			// Cold dark matter (CDM) density parameter:
			if (starts_with(current_line, "Omega_cdm =") || starts_with(current_line, "Omega_cdm=")){
				if(little_omegab_defined){
                    std::cout << "\n==> ERROR <==: Please don't mix definitions of capital and lower-case Omega's." << std::endl;
                    exit(1);
                }
                Omega_cdm = extract_value(current_line);
            }
			if (starts_with(current_line, "omega_cdm =") || starts_with(current_line, "omega_cdm=")){
				//if(!little_omegab_defined){
                //    std::cout << "\n==> ERROR <==: Please don't mix definitions of capital and lower-case Omega's." << std::endl;
                //    exit(1);
                //}
                Omega_cdm = extract_value(current_line);
                little_omegacdm_defined=true;
			}
			// Massive neutrinos:
			if (starts_with(current_line, "m_ncdm =") || starts_with(current_line, "m_ncdm=")){
				std::string token, value_str = current_line.substr(current_line.find("=")+1), delimiter = ",";
				size_t pos = 0;
				double tot_m_nu = 0.0;
				int cntr = 0;
				double m_nu[3] = {0.0, 0.0, 0.0};
				while ((pos = value_str.find(delimiter)) != std::string::npos) {
    				token = value_str.substr(0, pos);
					m_nu[cntr] = extract_value(token);
    				tot_m_nu += m_nu[cntr];
    				value_str.erase(0, pos + delimiter.length());
					cntr++;
				}
				m_nu[cntr] = extract_value(value_str);
				tot_m_nu += m_nu[cntr];

				if (m_nu[0] != m_nu[1] || m_nu[0] != m_nu[2] || m_nu[0] != m_nu[2]){
					std::cout << "\n--> WARNING <--: You have defined a non-degenerate hierarchy of neutrino mass\n" \
							  << "                 eigenstates. This hierarchy will be ignored. EuclidEmulator2 always\n" \
							  << "                 assumes a degenerate hierarchy of neutrino mass eigenstates." << std::endl;
				}
                CSM.Sum_m_nu.push_back(tot_m_nu);
				mnu_defined = true;
       		}
			if (starts_with(current_line, "Omega_ncdm =") || starts_with(current_line, "Omega_ncdm=")){
                Omega_ncdm_defined = true;
            }
			// spectral index:
			if (starts_with(current_line, "n_s =") || starts_with(current_line, "n_s=")){
                CSM.n_s.push_back(extract_value(current_line));
            }
			// DE EoS w_0:
			if (starts_with(current_line, "w0_fld =") || starts_with(current_line, "w0_fld=")){
                CSM.w_0.push_back(extract_value(current_line));
            }
			// DE EoS w_a:
			if (starts_with(current_line, "wa_fld =") || starts_with(current_line, "wa_fld=")){
                CSM.w_a.push_back(extract_value(current_line));
            }
			// spectral amplitude:
			if (starts_with(current_line, "A_s =") || starts_with(current_line, "A_s=")){
                CSM.A_s.push_back(extract_value(current_line));
				As_defined = true;
            }
			// redshifts:
			if (starts_with(current_line, "z_pk =") || starts_with(current_line, "z_pk=")){
				std::string token, value_str = current_line.substr(current_line.find("=")+1), delimiter = ",";
                size_t pos = 0;
				std::vector<double> z_tmp; // auxiliary vector (zvec is a matrix, z_tmp a row of this matrix)
				CSM.n_redshift.push_back(0);
				// Iterate through the list until only the last entry is left,
				// i.e. no more delimiters can be found:
                while ((pos = value_str.find(delimiter)) != std::string::npos) {
					CSM.n_redshift.at(0)++; //count the number of list items
                    token = value_str.substr(0, pos);
					z_tmp.push_back(atof(token.c_str()));
                    value_str.erase(0, pos + delimiter.length());
                }
				// Don't forget to add the last redshift in the list to the vector:
				CSM.n_redshift.at(0)++;
				z_tmp.push_back(atof(value_str.c_str()));
				CSM.zvec.push_back(z_tmp);
            }
			// Check parameters implicitly defined in EE2:
			if (starts_with(current_line, "Omega_k =") || starts_with(current_line, "Omega_k=")){
                if(extract_value(current_line) != 0.0 ){
					std::cout << "\n==> ERROR <==: The defined cosmology is non-flat. Please set Omega_k = 0.0." << std::endl;
					exit(1);
				}
            }
		}
		else{
            read_next_line = false;
        }
	}
	fclose(classfile);
	// Fill unspecified DE parameters:
	if (CSM.w_0.size() == 0){
		std::cout << "\n--> WARNING <--: w_0 is not specified --> set to -1.0." << std::endl; 
		CSM.w_0.push_back(-1.0);
	}
	if (CSM.w_a.size() == 0){
		std::cout << "\n--> WARNING <--: w_a is not specified --> set to 0.0." << std::endl;
		CSM.w_a.push_back(0.0);
	} 
	// Compute matter density parameter;
	CSM.Omega_m.push_back(Omega_cdm + CSM.Omega_b.at(0)); // Notice that here boh Omega and omega can be stored.
	// Convert from omega to Omega (if necessary):
	if (hubble_defined && little_omegab_defined) CSM.Omega_b.at(0) /= pow(CSM.h.at(0),2);
	if (hubble_defined && little_omegab_defined && little_omegacdm_defined) CSM.Omega_m.at(0) /= pow(CSM.h.at(0),2);
	// Check if all necessary parameters are defined:
	if (!hubble_defined){
        std::cout << "\n==> ERROR <==: You have not explicitely defined the Hubble parameter. Please do so! \n" \
                  << "               EuclidEmulator2 does not accept the peak scale parameter theta_s as an alternative\n" \
                  << "               to specifying H0 or h, respectively." << std::endl;
        exit(1);
    }
	if (!mnu_defined){
		if (Omega_ncdm_defined){
        	std::cout << "\n==> ERROR <==: You have not explicitely defined the neutrino mass eigenstates. Please do so! \n" \
            	      << "               Alternative specifications of the neutrino content (e.g. through Omega_ncdm) are not \n" \
					  << "               accepted by EuclidEmulator2." << std::endl;
        	exit(1);
		}else{
			std::cout << "\n--> WARNING <--: Neither neutrino mass eigenstates nor a neutrino density parameter have been\n" \
					  << "                 specified. Hence we assume only massless neutrinos: Sum m_nu = 0.0 eV." << std::endl;
			CSM.Sum_m_nu.push_back(0.0);
		}
    }
	if (!As_defined){
		std::cout << "\n==> ERROR <==: You have not explicitely defined the spectral amplitude A_s. Please do so! \n" \
				  << "               Alternative normalizations (as e.g. sigma_8) are not accepted by EuclidEmulator2." << std::endl;
		exit(1); 
	}
}

void read_cambfile(std::string camb_file_name, csmpars &CSM){
    printf("Reading cosmology from CAMB file...\n");
}

void read_parfile(std::string par_file_name, csmpars &CSM){
    printf("Reading cosmology from EE2 parameter file...\n");

	FILE * cosmofile;
    char instring[256];
    char * token;
        
	// Open parameter file (and check for success):
	cosmofile = fopen(par_file_name.c_str(), "r");
	if(cosmofile == NULL){
        std::cout << "Could not open cosmology file " <<  par_file_name << std::endl;
        std::cout << "Please make sure the command line argument list follows the format: " << std::endl;
        std::cout << std::endl;
        std::cout << "  ./ee2.exe [/path/to/cosmology/file]" << std::endl;
        std::cout << "or" << std::endl;
        std::cout << "  ./ee2.exe [Omega_b] [Omega_m] [Sum m_nu] [n_s] [h] [w_0] [w_a] [A_s] [z_1] ... [z_n]" << std::endl;
        exit(1);
    }

	// Fill arrays
	int linecntr = 0;
	bool read_next_line = true;
	while(read_next_line){  
		if (fgets(instring, 256, cosmofile) != NULL){   
			//Ignore the line if it is a comment
			if (instring[0] == '#'){
            	continue;
        	}
			//Remark: Don't forget to split (i.e. tokenize) each
			//        line into its individual parameters
			token = strtok(instring, ",");
			CSM.Omega_b.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.Omega_m.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.Sum_m_nu.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.n_s.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.h.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.w_0.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.w_a.push_back(atof(token));

			token = strtok(NULL, ",");
			CSM.A_s.push_back(atof(token));

			// Read the rest of the line as individual redshifts:
			std::vector<double> z_tmp;
			bool reached_end_of_line = false;
			while (!reached_end_of_line){   
				token = strtok(NULL, ",");
				printf("Redshifts: %s\n", token);
				z_tmp.push_back(atof(token));
				if (strstr(token, "\n") != NULL) reached_end_of_line = true;
			}
			CSM.zvec.push_back(z_tmp);
			CSM.n_redshift.push_back(z_tmp.size()); 
			linecntr++;
		}
		else{
			read_next_line = false;
		}   
	}
	fclose(cosmofile);
}
