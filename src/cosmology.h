#ifndef COSMOLOGY_H
#define COSMOLOGY_H

class Cosmology{
	public:
		double Omega_b;
	    double Omega_m;
		double Sum_m_nu;
    	double n_s;
    	double h;
    	double w_0;
		double w_a;
    	double A_s;

		Cosmology();
		void read_from_file(char *filename);
		void read_csm();
};

#endif
