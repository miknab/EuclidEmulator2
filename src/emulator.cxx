#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
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
}

// Compute NLC
void EuclidEmulator::compute_nlc(Cosmology csm, double *nlc){
}
	
// Write result to filei
void EuclidEmulator::write_nlc(double* nlc){
}
