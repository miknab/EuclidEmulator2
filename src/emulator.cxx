#include <iostream>
#include <string>
#include <sys/stat.h> // struct stat and fstat() function
#include <sys/mman.h> // mmap() function
#include <fcntl.h>    // declaration of O_RDONLY
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
	/// VARIABLE DECLARATIONS ///
	off_t size;
    struct stat s;
	double *data;
	int i, idx = 0;

	// ==== LOAD EUCLIDEMULATOR2 DATA FILE ==== //
	int fp = open("./ee2_bindata.dat", O_RDONLY);
	if(!fp) {
		cerr << "Unable to open ./ee2_data/ee2_pcs.dat.\n";
        exit(1);
	}

	// Get the size of the file. //
    printf("Get file size...\n");
    int status = fstat(fp, & s);
    size = s.st_size;

	// Map the file into memory //
	data = (double *) mmap (0, size, PROT_READ, MAP_PRIVATE, fp, 0);

	// Reading in principal components //
	for (i=0;i<15;++i) {
    	this->pc[i] = &data[idx];  // pc[0] = PCA mean
    	idx += nk*nz;
    }

	// Reading in PCE coefficients //
	for (i=0;i<14;++i) {
    	this->pce_coeffs[i] = &data[idx];
    	idx += n_coeffs[i];
    }

	// Reading in PCE multi-indices //
	for (i=0;i<14;++i) {
    	this->pce_multiindex[i] = &data[idx];
    	idx += 8*n_coeffs[i];
    }
}

// Compute NLC
void EuclidEmulator::compute_nlc(Cosmology csm, double *nlc){
	
}
	
// Write result to file
void EuclidEmulator::write_nlc(double* nlc){
}
