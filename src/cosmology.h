#ifndef COSMOLOGY_H
#define COSMOLOGY_H

class Cosmology{
	public:
		double cosmo[8];

		/* Ranges for cosmological parameters */
        const double minima[8] = {0.04, 0.24, 0.00, 0.92, 0.61, -1.3, -0.7, 1.7e-9};
        const double maxima[8] = {0.06, 0.40, 0.15, 1.00, 0.73, -0.7, 0.5, 2.5e-9};
		
		/* Member functions */
		Cosmology(double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s);
		void read_from_file(char *filename);

	private:
		double cosmo_tf[8];
		void isoprob_tf();
		void check_parameter_ranges();
};

#endif
