#ifndef EMULATOR_H
#define EMULATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>
#include <fstream>
#include "cosmology.h"

using namespace std;

class EuclidEmulator{
	private:
		/* Private members */
		const int nz; // number of redshifts in the training data
		const int nk; // number of k modes in training data
		const int n_coeffs[14];

    	gsl_spline2d     *logklogz2pc_spline[15];

		/* Private data containers */
		double *pc[15];
		double pc_weights[14];
		double *pce_coeffs[14];
		double *pce_multiindex[14];

		/* Private member functions */
		void read_in_ee2_data_file();
		void pc_2d_interp();
		
	public:
		/* Public members */
		double kvec[613];
		double Bvec[613];

		/* Public member functions */
		EuclidEmulator();
		~EuclidEmulator();
		void compute_nlc(Cosmology csm, double *redshift, double *kmode);
		void write_nlc(double* nlc);
};

#endif
