#ifndef EMULATOR_H
#define EMULATOR_H

#include "cosmology.h"

class EuclidEmulator{
	private:
		const int nz; // number of redshifts in the training data
		const int nk; // number of k modes in training data
		const int nCoeffs[14];
		
		/* Data containers */
		double *pc_mean;
		double *pc;
		double *pc_weights[14];
		double *pce_coeffs[14];
		double *pce_multiindex[14];
			
	public:
		EuclidEmulator();
		void compute_nlc(Cosmology csm, double *nlc);
		void write_nlc(double* nlc);
};

#endif
