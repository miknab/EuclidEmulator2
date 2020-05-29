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
        ("b,Omega_b", "baryon density parameter", cxxopts::value< std::vector<double> >())
        ("m,Omega_m", "(total) matter density parameter", cxxopts::value< std::vector<double> >())
        ("s,Sum_m_nu", "sum of neutrino masses in eV", cxxopts::value< std::vector<double> >())
        ("n,n_s", "spectral index", cxxopts::value< std::vector<double> >())
        ("H,hubble", "dimensionless Hubble parameter", cxxopts::value< std::vector<double> >())
        ("W,w_0", "time-independent dark energy equation-of-state parameter", cxxopts::value< std::vector<double> >())
        ("w,w_a", "time-dependent dark energy equation-of-state parameter", cxxopts::value< std::vector<double> >())
        ("A,A_s", "spectral amplitude", cxxopts::value< std::vector<double> >())
        ("z,redshift", "redshift at which the NLC shall be computed", cxxopts::value< std::vector< std::vector<double> > >())
        ("l,classfile", "path to CLASS parameter file", cxxopts::value<std::string>())
        ("a,cambfile", "path to CAMB parameter file", cxxopts::value<std::string>())
        ("p,parfile", "path to EE2 parameter file", cxxopts::value<std::string>())
        ("o,outfile", "path to output file", cxxopts::value<std::string>())
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

void read_cosmo_from_cmdline(cxxopts::ParseResult result, csmpars CSM){
	printf("Reading cosmology from command line...\n");

	if (result.count("Omega_b")){
        CSM.Omega_b = result["Omega_b"].as< std::vector<double> >();
    }
    if (result.count("Omega_m")){
        CSM.Omega_m = result["Omega_m"].as< std::vector<double> >();
    }
    if (result.count("Sum_m_nu")){
        CSM.Sum_m_nu = result["Sum_m_nu"].as< std::vector<double> >();
    }
    if (result.count("n_s")){
        CSM.n_s = result["n_s"].as< std::vector<double> >();
    }
    if (result.count("hubble")){
        CSM.h = result["hubble"].as< std::vector<double> >();
    }
    if (result.count("w_0")){
        CSM.w_0 = result["w_0"].as< std::vector<double> >();
    }
	if (result.count("w_a")){
        CSM.w_a = result["w_a"].as< std::vector<double> >();
    }
    if (result.count("A_s")){
        CSM.A_s = result["A_s"].as< std::vector<double> >();
    }
    if (result.count("redshift")){
		std::string zvecstr = "zvec = [";
		std::vector<double> z_tmp;
		CSM.n_redshift.push_back(result.count("redshift"));
        CSM.zvec.push_back(result["redshift"].as<std::vector<double> >()); 
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

void read_classfile(std::string class_file_name, csmpars CSM){
    printf("Reading cosmology from CLASS file...\n");
}

void read_cambfile(std::string camb_file_name, csmpars CSM){
    printf("Reading cosmology from CAMB file...\n");
}

void read_parfile(std::string par_file_name, csmpars CSM){
    printf("Reading cosmology from EE2 parameter file...\n");
}
