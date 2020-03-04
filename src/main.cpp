#include <iostream>
#include <string>
#include <hdf5.h>
#include <hdf5_hl.h>

class Cosmology{
	public:
		double Omega_b;
	    double Omega_m;
		double Sum_m_nu;
    	double n_s;
    	double h;
    	double w_0;
		double w_a;
    	double A_s;

		Cosmology();
		void read_from_file(char *filename);
		void read_csm();
};

Cosmology::Cosmology(){}

void Cosmology::read_from_file(char *filename){}

void Cosmology:: read_csm(){}


class LegendreMultiVar{
	/* class to compute the multivariate legendre polynomials */
};

class EuclidEmulator{
	/* ===  Data members === */
	private:
		double* PC1;
	
	public:
		EuclidEmulator();
		double* compute_nlc(Cosmology csm);
		void write_nlc(double* nlc);
};

EuclidEmulator::EuclidEmulator(){	/* === Member function === */
	// Constructor loading PCE data file
	hid_t file;
	hsize_t size_PC1;

		file = H5Fopen("EE2_cut1_zStpStart60.hdf5", H5F_ACC_RDONLY, H5P_DEFAULT);
		//if (H5LTget_dataset_info(file, "/PCA/PC1", &size_PC1, NULL, NULL) < 0) abort();
		if (H5LTread_dataset_double(file, "/PCA/PC1", PC1) < 0) abort();
}

// Compute NLC
double* EuclidEmulator::compute_nlc(Cosmology csm){}
	
// Write result to filei
void EuclidEmulator::write_nlc(double* nlc){
}

int main(int argc, char *argv[]) {
	const int nPCA = 15; //15 principal components
	double* nlc;
	/* Initialize EE session */
	EuclidEmulator ee2 = EuclidEmulator();

	/* Read-in cosmology/cosmologies */
	Cosmology mycsm = Cosmology();

	/* compute NLCs for each cosmology */
	int csm_cntr = 0;
	nlc = ee2.compute_nlc(mycsm);
	return 0;
}
