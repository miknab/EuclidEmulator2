#include <iostream>
#include <string>
#include <hdf5.h>
#include <hdf5_hl.h>
#include "cosmology.h"
#include "pce.h"
#include "emulator.h"

EuclidEmulator::EuclidEmulator(){	/* === Member function === */
	// Constructor loading PCE data file
	hid_t file;
	hsize_t size_PC1;

		file = H5Fopen("EE2_cut1_zStpStart60.hdf5", H5F_ACC_RDONLY, H5P_DEFAULT);
		//if (H5LTget_dataset_info(file, "/PCA/PC1", &size_PC1, NULL, NULL) < 0) abort();
		if (H5LTread_dataset_double(file, "/PCA/PC1", PC1) < 0) abort();
}

// Compute NLC
void EuclidEmulator::compute_nlc(Cosmology csm, double *nlc){
}
	
// Write result to filei
void EuclidEmulator::write_nlc(double* nlc){
}
