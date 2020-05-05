#include <iostream>
#include <assert.h>
#include <math.h>
#include "cosmo.h"
#include "units_and_constants.h"

#define EPSCOSMO 1e-7
#define LIMIT 1000

/* CONSTRUCTOR */
Cosmology::Cosmology(double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s):
	nSteps(101),
	nTable(101)
	{
	this->cosmo[0] = Omega_b;
    this->cosmo[1] = Omega_m;
    this->cosmo[2] = Sum_m_nu;
    this->cosmo[3] = n_s;
    this->cosmo[4] = h;
    this->cosmo[5] = w_0;
    this->cosmo[6] = w_a;
	this->cosmo[7] = A_s;

	// Compute the actual critical density of the Universe in SI units for
	// the given cosmology:
	SI_units::rho_crit = SI_units::rho_crit_over_h2 * pow(this->cosmo[4],2);

	// GSL interpolation object for redshift-to-nStep conversion:
	// The spline is depends on the cosmology but takes redshift as an
	// argument. For this very reason, the spline can computed once when
	// a Cosmology class object is instantiated. From then on, the spline
	// can be evaluated as often as necessary until the Cosmology object 
	// is destroyed.
	acc = gsl_interp_accel_alloc();
    z2nStep_spline = gsl_spline_alloc(gsl_interp_cspline, nTable);
	// In the next line the spline is actually being computed:
	compute_z2nStep_spline();

	t0  = a2t(1.0); // proper time at z = 0 (or equivalently a = 1) 
    t10 = a2t(10.0); // proper time at z = 10 (or equivalently a = 0.090909...)
    Delta_t = (t0-t10)/nSteps;

	/* Member functions */
	check_parameter_ranges();
	isoprob_tf();
	//convert_to_pkdgrav();
	print_cosmo();
	print_cosmo_tf();
}

/* DESTRUCTOR */
Cosmology::~Cosmology(){
    gsl_interp_free(spline);
}

/* CHECK PARAMETER RANGES */
void Cosmology::check_parameter_ranges(){
    for(int i=0 ; i<8; i++) {
        if(cosmo[i] < minima[i] || cosmo[i] > maxima[i] ) {
			std::cout << "Parameter " << i << " is outside allowed range:" << std::endl;
			std::cout << "cosmo[" << i << "] = " << cosmo[i] << std::endl;
			exit(1);
		}
		if(i==5) i++;
    }
	// Check w_a separately because values >0.5 are not
	// allowed although they are inside the range  
	if(cosmo[6] < -0.7 || cosmo[6] > 0.5 ) {
        std::cout << "Parameter w_a is outside allowed range:" << std::endl;
        std::cout << " w_a = " << cosmo[6] << std::endl;
        exit(1);
    }
}

/* ISOPROBALISTIC TRANSFORMATION TO UNIT HYPERCUBE */
void Cosmology::isoprob_tf(){
	for (int i=0; i<8; i++){
		cosmo_tf[i] = 2*(cosmo[i] - minima[i])/(maxima[i] - minima[i]) - 1.0;
	}
}

/* COMPUTE a(t) TABLE FOR GIVEN COSMOLOGY */
double Cosmology::Omega_matter(double a){ 
	/* This function computes the matter density parameter *\
	\* in the Universe at scale factor a.                  */
	return this->cosmo[1] / (a*a*a);
}

double Cosmology::Omega_gamma(double a){
	/* This function computes the photon density parameter in the Universe at scale factor a */

	// Present day photon density corresponding to T_gamma (also in SI units):
	double rho_gamma_0 =  M_PI * M_PI / 15.0 
						* pow(SI_units::kB,4) / (pow(SI_units::hbar,3)*pow(SI_units::c,5))
						* pow(SI_units:Tgamma,4);

	// Scale the photon density and return result
    return rho_gamma_0 / (SI_units::rho_crit * a*a*a*a);
}

double Cosmology::rho_nu_i_integrand(double p, void * params){
	/* This function provides the integrand for the function Cosmology::Omega_nu */
	rho_nu_parameters *rho_nu_pars = reinterpret_cast<rho_nu_parameters *>(params);

	double Tnu = pow(Neff/3.0,0.25)*pow(4.0/11.0,1./3.)*SI_units::Tgamma;
	double prefactor = 1.0/(pow(M_PI,2) * pow(SI_units::hbar,3) * pow(SI_units::c,2));
	double p2 = p*p;

	return  SI_units::c * p2 * sqrt( pow(rho_nu_pars->mnu * SI_units::c,2) + p2 )
		  / (exp(rho_nu_pars->a * p/Tnu * SI_units::c/SI_units::kB) + 1);
}

double Cosmology::Omega_nu(double a){
	/* This function computes the neutrino density parameter in the Universe at scale factor a */
	/* REMARK: The assumption of a degenerate neutrino hierarchy is hardcoded. */

	rho_nu_parameters rho_nu_pars;
	gsl_function F;
	double rho_nu_i, error;

	//the letter "i" in "rho_nu_i" emphasizes that this variable
	//contains the density of only ONE neutrino species         

	// Compute the neutrino density of ONE neutrino species of mass m_nu
	// (according to eq. 2.69 in my thesis). Remember that this integral is 
	// an integral over the momentum p, so we start by defining an uper bound
	// "pmax" for the integration:
	double pmax = 0.004/rho_nu_pars->a * SI_units::eV/SI_units::c;

	gsl_integration_workspace *gsl_wsp = gsl_integration_workspace_alloc(LIMIT);

	// Remember that we always assume degenerate neutrino hierarchy. This is
	// why the mass of a single neutrino species is a third of Sum m_nu (which
	// is stored in this->cosmo[2].
	rho_nu_pars.mnu = this->cosmo[2]/3.0; 
	rho_nu_pars.a = a;

	F.function = &rho_nu_i_integrand;
	F.params = &rho_nu_pars;

	gsl_integration_qag(&F, 0, pmax, 0.0,
						EPSCOSMO, LIMIT, GSL_INTEG_GAUSS61,
						gsl_wsp, &rho_nu_i, &error); 
	gsl_integration_romberg_free(gsl_wsp); 

    // NOTICE: The prefactor 3 comes from the fact that we ALWAYS consider three
    // equal-mass neutrinos while rho_nu_i computes the density of ONE neutrino species
	return 3*rho_nu_i / SI_units::rho_crit;
}

double Cosmology::Omega_DE(double a){ 
	/* This function computes the dark energy (DE) density parameter of   *\
	|* the Universe. This computation is based on the flatness condition: *|
	|*																	  *|
	|*			Omega_matter + Omega_gamma + Omega_nu + Omega_DE = 1      *|
	|*																	  *|
	\* The assumption of a flat Universe is thus hardcoded.               */
	double Omega_DE_0 = 1 - (this->cosmo[0] + Omega_gamma_0 + Omega_nu_0);
	double w_0 = this->cosmo[5];
	double w_a = this->cosmo[6];

	// The following is equation (2.67) in my thesis
	return Omega_DE_0 * pow(a, -3.0 *(1 + w_0 + w_a)) * exp(-3*(1-a)*w_a);
}

void Cosmology::a2Hubble(double a){
	/* This function computes the Hubble parameter at scale factor a */
	H0 = 100*this->cosmo[4];
	return H0 * sqrt( Cosmology::Omega_matter(a)
					+ Cosmology::Omega_gamma(a)
					+ Cosmology::Omega_nu(a)
					+ Cosmology::Omega_DE(a)
					);
}

double Cosmology::a2t_integrand(double a){
	/* This function provides the integrand for the GSL *\
	\* integration in Cosmology::a2t below.             */
	return 2.0/(3.0 * pow(a, 1.5) * a2Hubble(a)) 
}

double Cosmology::a2t(double a){
	/* This function converts a scale factor a to a proper time. *\
	|* The integration is solved by GSL and the integrand is     *|
	\* by Cosmology::a2t_integrand.                              */
	gsl_function F;
	gsl_integration_workspace *gsl_wsp = gsl_integration_workspace_alloc(LIMIT);
	double result, error;

	F.function = &a2t_integrand;

	gsl_integration_qag(&F, 0.0, a, 0.0,
						EPSCOSMO, LIMIT, GSL_INTEG_GAUSS61,
						gsl_wsp, &result, &error);
	
	return result;
}

void Cosmology::compute_z2nStep_spline(){
	/* This function creates and interpolates the array of (z,nStep)-tuples *\
	|* computed in "Cosmology::compute_time_lookup_table". It returns a     *|
	\* gsl_spline function than can be readily evaluated.                   */

	// Step 1: Setup the array
	double avec[this->nTable]={0};
	double frac_nStep[this->nTable]={0};

	double z10 = 10.0;
	for(int idx=0; idx<nTable; idx++){
		// Loop through redshifts: z \in {10.0, 9.9, ..., 0.1, 0.0}
		z = z10 - idx*0.1; 
		// Convert z to a (remember that GSL interpolator expects
		// x-values to be in ascending order) 
		avec[idx] = 1.0/(z+1.0);
		// Convert a to t
		t_current = Cosmology::a2t(avec[idx]);
		// Convert t to nStep (fractional)
		frac_nStep[idx] = (t_current - this->t10)/this->Delta_t;
	}

	// Some sanity checks:
	assert(frac_nStep[0] == 0);
	assert(frac_nStep[this->nTable-1] == this->nSteps);

	// Step 2: Interpolate the array
	gsl_spline_init(this->z2nStep_spline, avec, frac_nStep, this->nTable);
}

/* INTERPOLATE OUTPUT STEP */
double Cosmology::compute_step_number(double z);
	/* This function evaluates the spline mapping *\
	\* a redshift to a (fractional) output step.  */ 
	if(z==0.0){
		return 101.0;
	}
	else{
		// Now simply evaluate the precomputed spline
		return gsl_spline_eval(this->z2nStep_spline, z, this->acc);
	}
}

/* PRINT FUNCTIONS */
void Cosmology::print_cosmo(){
	std::cout << "The current cosmology is defined as:" << std::endl;
	std::cout << std::endl;
	std::cout << "\tOmega_b = " << this->cosmo[0] << std::endl;
	std::cout << "\tOmega_m = " << this->cosmo[1] << std::endl;
	std::cout << "\tSum m_nu = " << this->cosmo[2] << std::endl;
	std::cout << "\tn_s = " << this->cosmo[3] << std::endl;
	std::cout << "\th = " << this->cosmo[4] << std::endl;
	std::cout << "\tw_0 = " << this->cosmo[5] << std::endl;
	std::cout << "\tw_a = " << this->cosmo[6] << std::endl;
	std::cout << "\tA_s = " << this->cosmo[7] << std::endl;
	std::cout << std::endl;
}

void Cosmology::print_cosmo_tf(){
	std::cout << "The current cosmology is mapped to:" << std::endl;
    std::cout << std::endl;
    std::cout << "\ttf(Omega_b) = " << this->cosmo_tf[0] << std::endl;  
    std::cout << "\ttf(Omega_m) = " << this->cosmo_tf[1] << std::endl;
    std::cout << "\ttf(Sum m_nu) = " << this->cosmo_tf[2] << std::endl;
    std::cout << "\ttf(n_s) = " << this->cosmo_tf[3] << std::endl;
    std::cout << "\ttf(h) = " << this->cosmo_tf[4] << std::endl;
    std::cout << "\ttf(w_0) = " << this->cosmo_tf[5] << std::endl;
    std::cout << "\ttf(w_a) = " << this->cosmo_tf[6] << std::endl;
    std::cout << "\ttf(A_s) = " << this->cosmo_tf[7] << std::endl;
	std::cout << std::endl;
}

/* READ COSMOLOGY FROM FILE  */
void read_from_file(char *filename){}

