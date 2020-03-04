#ifndef EMULATOR_H
#define EMULATOR_H

#include "cosmology.h"

class EuclidEmulator{
	private:
		double* PC1;
	
	public:
		EuclidEmulator();
		void compute_nlc(Cosmology csm, double *nlc);
		void write_nlc(double* nlc);
};

#endif
