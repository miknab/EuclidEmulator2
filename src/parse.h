#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <cxxopts.hpp>

void read_cosmo_from_cmdline();//double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s, vector<double> z);

void read_classfile(string class_file_name);//d, double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s, vector<double> z);

void read_cambfile(string camb_file_name);//d, double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s, vector<double> z);

void read_parfile(string par_file_name);//d, double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s, vector<double> z);
