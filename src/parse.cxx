#include <parse.h>
#include <assert.h>

void parser(int n_args, char * vec_args[]){
	size_t n_cosmo_pars;
	// Option 1: pass in a cosmology following the format:
    //      Omega_b, Omega_m, Sum_m_nu, n_s, h, w_0, w_a, A_s, z[0], ..., z[n]
        // yet to be added

    // Option 2: pass in a cosmology, a camb, a class or a parameter file
    // define the Options class and add the individual options to it
    cxxopts::Options options("EuclidEmulator2", "Highly accurate and efficient AI-based \
predictor of the non-linear correction of the matter power spectrum.");
    options.add_options()
        ("b,Omega_b", "baryon density parameter", cxxopts::value<double>())
        ("m,Omega_m", "(total) matter density parameter", cxxopts::value<double>())
        ("s,Sum_m_nu", "sum of neutrino masses in eV", cxxopts::value<double>())
        ("n,n_s", "spectral index", cxxopts::value<double>())
        ("H,hubble", "dimensionless Hubble parameter", cxxopts::value<double>())
        ("W,w_0", "time-independent dark energy equation-of-state parameter", cxxopts::value<double>())
        ("w,w_a", "time-dependent dark energy equation-of-state parameter", cxxopts::value<double>())
        ("A,A_s", "spectral amplitude", cxxopts::value<double>())
        ("z,redshift", "redshift at which the NLC shall be computed", cxxopts::value<std::vector<double>>())
        ("l,classfile", "path to CLASS parameter file", cxxopts::value<std::string>())
        ("a,cambfile", "path to CAMB parameter file", cxxopts::value<std::string>())
        ("p,parfile", "path to EE2 parameter file", cxxopts::value<std::string>())
        ("o,outfile", "path to output file", cxxopts::value<std::string>())
        ("v,verbose", "verbose output", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage");

	// now actually pars the arguments
	auto result = options.parse(n_arg, vec_args);
	
	if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }
	bverbose = result["verbose"].as<bool>();

    if (result.count("outfile")){
        outfilename = result["outfile"].as<std::string>();
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
		read_cosmo_from_cmdline(result);
	}
	if (result.count("classfile")){
		assert(n_cosmo_pars == 0);
       	read_classfile(result["classfile"].as<std::string>());//, Omega_b[0], Omega_m[0], Sum_m_nu[0], n_s[0], h[0], w_0[0], w_a[0], A_s[0], z[0]);
	   }
    if (result.count("cambfile")){
		assert(n_cosmo_pars == 0);
       	read_clambfile(result["cambfile"].as<std::string>());//, Omega_b[0], Omega_m[0], Sum_m_nu[0], n_s[0], h[0], w_0[0], w_a[0], A_s[0], z[0]);
    }
    if (result.count("parfile")){
		assert(n_cosmo_pars == 0);
       	read_parfile(result["parfile"].as<std::string>());//, Omega_b[0], Omega_m[0], Sum_m_nu[0], n_s[0], h[0], w_0[0], w_a[0], A_s[0], z[0]);
   	}
}

void read_cosmo_from_cmdline(auto result){
	printf("Reading cosmology from command line...\n");
	if (result.count("Omega_b")){
        Omega_b[0] = result["Omega_b"].as<double>();
    }
    if (result.count("Omega_m")){
        Omega_m[0] = result["Omega_m"].as<double>();
    }
    if (result.count("Sum_m_nu")){
        Sum_m_nu[0] = result["Sum_m_nu"].as<double>();
    }
    if (result.count("n_s")){
        n_s[0] = result["n_s"].as<double>();
    }
    if (result.count("hubble")){
        h[0] = result["hubble"].as<double>();
    }
    if (result.count("w_0")){
        w_0[0] = result["w_0"].as<double>();
    }
	if (result.count("w_a")){
        w_a[0] = result["w_a"].as<double>();
    }
    if (result.count("A_s")){
        A_s[0] = result["A_s"].as<double>();
    }
    if (result.count("redshift")){
        n_redshift[0] = result.count("redshift");
        std::vector<double> redshiftvector (n_redshift[0]);
        redshiftvector = result["redshift"].as<std::vector<double>>();
        string zvecstr = "zvec = [";
        for(int i=0; i<n_redshift[0]; i++){
            zvec[0][i] = redshiftvector[i];
            zvecstr.append(to_string(zvec[0][i]));
            if (i != n_redshift[0]-1) {
                zvecstr.append(", ");
            }
        }
        zvecstr.append("]");
        cout << zvecstr << endl;
    }
}

void read_classfile(string class_file_name){
    printf("Reading cosmology from CLASS file...\n");
}

void read_cambfile(string camb_file_name){
    printf("Reading cosmology from CAMB file...\n");
}

void read_parfile(string par_file_name){
    printf("Reading cosmology from EE2 parameter file...\n");
}
