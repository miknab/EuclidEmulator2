#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <cxxopts.hpp>

struct csmpars {
	int verbosity_level;
	std::vector<double> Omega_b;
    std::vector<double> Omega_m;
    std::vector<double> Sum_m_nu;
    std::vector<double> n_s;
    std::vector<double> h;
    std::vector<double> w_0;
    std::vector<double> w_a;
    std::vector<double> A_s;
    std::vector<int> n_redshift;
    std::vector< std::vector<double> > zvec;
	std::string outfilename;
};

csmpars ee2_parser(int n_args, char * vec_args[]);
void read_cosmo_from_cmdline(cxxopts::ParseResult result, csmpars CSM);
void read_classfile(std::string class_file_name, csmpars CSM);
void read_cambfile(std::string camb_file_name, csmpars CSM);
void read_parfile(std::string par_file_name, csmpars CSM);
