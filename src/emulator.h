#ifndef EMULATOR_H
#define EMULATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>
#include <fstream>
#include <vector>
#include "cosmo.h"

using namespace std;

class EuclidEmulator{
	private:
		/* Private members */
		const int nz; // number of redshifts in the training data
		const int nk; // number of k modes in training data
		const int n_coeffs[14];
		const int lmax;

		gsl_interp_accel * logk2pc_acc[15];
    	gsl_interp_accel * logz2pc_acc[15];
    	gsl_spline2d     * logklogz2pc_spline[15];

		/* Private data containers */
		double * pc[15];             // principal components
		double pc_weights[14];      // PCA weights
		double * pce_coeffs[14];     // PCE coefficients
		double * pce_multiindex[14]; // PCE multi-indices
		double * univ_legendre[8]; // univariate legendre polynomials
		double * pce_basisfuncs;		// multivariate legendre polynomials

		/* Private member functions */
		void read_in_ee2_data_file();
		void pc_2d_interp();
		void print_info();		

	public:
		/* Public members */
		double kvec[613];
		double Bvec[101][613];

		/* Public member functions */
		EuclidEmulator();
		~EuclidEmulator();
		void compute_nlc(Cosmology csm, vector<double> redshift, int n_redshift);
		void write_nlc2file(const string &filename, vector<double> zvec, int n_redshift);
};

#endif
