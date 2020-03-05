#include <iostream>
#include <string>

#include "cosmology.h"
#include "pce.h"
#include "emulator.h"

EuclidEmulator::EuclidEmulator(): 
	nz(101),
	nk(613),
	nCoeffs{53, 53, 117, 117, 53, \
			117, 117, 117, 117, 521, \
			117, 1539, 173, 457}
{
	/* === Member function === */
	// Constructor loading PCE data file

}

// Compute NLC
void EuclidEmulator::compute_nlc(Cosmology csm, double *nlc){
}
	
// Write result to filei
void EuclidEmulator::write_nlc(double* nlc){
}
