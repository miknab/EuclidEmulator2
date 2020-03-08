#include <iostream>
#include <string>
#include <sys/stat.h> // struct stat and fstat() function
#include <sys/mman.h> // mmap() function
#include <fcntl.h>    // declaration of O_RDONLY
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>
#include <math.h>
#include "cosmology.h"
#include "pce.h"
#include "emulator.h"

using namespace std; 

/* CONSTRUCTOR */
EuclidEmulator::EuclidEmulator(): 
	nz(101),
	nk(613),
	n_coeffs{53, 53, 117, 117, 53, \
			117, 117, 117, 117, 521, \
			117, 1539, 173, 457}
	{
	read_in_ee2_data_file();
	pc_2d_interp();
}

/* DESTRUCTOR */
EuclidEmulator::~EuclidEmulator(){
	for(int i=0; i<15; i++) gsl_spline2d_free(logklogz2pc_spline[i]);
}

/* FUNCTION TO READ IN THE DATA FILE */
void EuclidEmulator::read_in_ee2_data_file(){	
	/// VARIABLE DECLARATIONS ///
	off_t size;
    struct stat s;
	double *data;
	double *kptr;
	int i, idx = 0;

	for(i=0; i < 613; i++){
		this->Bvec[i] = 0.0; // initialize the resulting nlc vector
	}

	// ==== LOAD EUCLIDEMULATOR2 DATA FILE ==== //
	int fp = open("./ee2_bindata.dat", O_RDONLY);
	if(!fp) {
		cerr << "Unable to open ./ee2_bindata.dat\n";
        exit(1);
	}

	// Get the size of the file. //
    printf("Get file size...\n");
    int status = fstat(fp, & s);
    size = s.st_size;

	// Map the file into memory //
	data = (double *) mmap (0, size, PROT_READ, MAP_PRIVATE, fp, 0);

	// Reading in principal components //
	for (i=0;i<15;i++) {
    	this->pc[i] = &data[idx];  // pc[0] = PCA mean
    	idx += nk*nz;
    }

	// Reading in PCE coefficients //
	for (i=0;i<14;i++) {
    	this->pce_coeffs[i] = &data[idx];
    	idx += n_coeffs[i];
    }

	// Reading in PCE multi-indices //
	for (i=0;i<14;i++) {
    	this->pce_multiindex[i] = &data[idx];
    	idx += 8*n_coeffs[i];
    }

	// vector of k modes
    kptr = &data[idx];
    for (i=0;i<nk;i++) {
		this->kvec[i] = kptr[i];
    }
    idx += nk;

	// Check if all data has been read in properly
	assert(idx == size/sizeof(double));
}

/* 2D INTERPOLATION OF PRINCIPAL COMPONENTS */
void EuclidEmulator::pc_2d_interp(){
	double logk[nk];
	double stp[nz];
	int i;
	
	for (i=0; i<nk; i++) logk[i] = log(kvec[i]);
	for (i=nz-1; i>=0; i--) stp[i] = i;	

	for (int i=0; i<15; i++){
    	logklogz2pc_spline[i] = gsl_spline2d_alloc(gsl_interp2d_bicubic, nk, nz);
    	gsl_spline2d_init(logklogz2pc_spline[i], logk, stp, pc[i], nk, nz);
	}
}

/* COMPUTE NLC */
void EuclidEmulator::compute_nlc(Cosmology csm, double* redshift, double *kmode){
	// Somewhere here the following lines has to be executed
	//for(int i=0; i<nz; i++){
	//	for(int j=0; j<nk; j++){
    //		gsl_spline2d_eval(logklogz2pc_spline, log(kmode[j]), log(redshift[i]), logk2pc_acc, logz2pc_acc);
	//	}
	//}
	// assign to this->Bvec
}
	
/* WRITE NLC TO FILE */
void EuclidEmulator::write_nlc(double* nlc){
}
