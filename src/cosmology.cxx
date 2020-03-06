#include <iostream>
#include <assert.h>
#include "cosmology.h"

/* CONSTRUCTOR */
Cosmology::Cosmology(double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s){
	this->cosmo[0] = Omega_b;
    this->cosmo[1] = Omega_m;
    this->cosmo[2] = Sum_m_nu;
    this->cosmo[3] = n_s;
    this->cosmo[4] = h;
    this->cosmo[5] = w_0;
    this->cosmo[6] = w_a;
	this->cosmo[7] = A_s;

	check_parameter_ranges();
	isoprob_tf();
}


/* CHECK PARAMETER RANGES */
void Cosmology::check_parameter_ranges(){
    for(int i=0 ; i<8; i++) {
        if(cosmo[i] < minima[i] || cosmo[i] > maxima[i] ) {
			std::cout << "Parameter " << i << " is outside allowed range:" << std::endl;
			std::cout << "cosmo[" << i << "] = " << cosmo[i] << std::endl;
			exit(1);
		}
    }
}

/* ISOPROBALISTIC TRANSFORMATION TO UNIT HYPERCUBE */
void Cosmology::isoprob_tf(){
	for (int i=0; i<8; i++){
		cosmo_tf[i] = 2*(cosmo[i] - minima[i])/(maxima[i] - minima[i]) - 1.0;
	}
}
/* READ COSMOLOGY FROM FILE  */
void read_from_file(char *filename){}

