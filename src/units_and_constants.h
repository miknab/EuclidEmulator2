#ifndef UNITS_AND_CONSTANTS
#define UNITS_AND_CONSTANTS
#include <math.h>

struct SI_units{
	// Base units
	static const double second; 
	static const double meter; 
	static const double kilogram; 
	static const double Kelvin;

	// derived units
	static const double Joule;
	static const double Msol;	// in kilogram
	static const double Lbox;   // in megaparsec
	static const double Tgamma; // in Kelin
	static const double Mpc; // megaparsec

	// natural constant
	static const double c; // speed of light
	static const double G; // Newton's constant of gravity
	static const double kB; // Boltzmann constant
	static const double eV; // electron Volt
	static const double hbar; // reduced Planck constant
	static const double rho_crit_over_h2; // critical density of the Universe multiplied by h^2
	static double rho_crit; // critical density of the Universe
};

// Base units
const double SI_units::second   = 1.0; // in seconds
const double SI_units::meter    = 1.0; // in meters
const double SI_units::kilogram = 1.0; // in kilogram
const double SI_units::Kelvin   = 1.0; // in Kelvin

// derived units
const double SI_units::Joule = SI_units::kilogram * pow(SI_units::meter,2)/pow(SI_units::second,2); // Joule
const double SI_units::Mpc   = 3.085677581282e22 * SI_units::meter; // megaparsec
const double SI_units::Msol  = 1.98841e30 *SI_units::kilogram; // solar SI_units::kilogram

// natural constant
const double SI_units::c        = 2.99792458 * SI_units::meter/SI_units::second; // speed of light
const double SI_units::G        = 6.67428e-11 * pow(SI_units::meter,3)/(SI_units::kilogram*pow(SI_units::second,3)); // Newton's constant of gravity
const double SI_units::kB       = 1.3806504e-23 * SI_units::Joule/SI_units::Kelvin; // Boltzmann constant
const double SI_units::eV       = 1.602176487e-19 * SI_units::Joule; // electron Volt
const double SI_units::hbar     = 6.62606896e-34/(2*M_PI) * SI_units::Joule * SI_units::second; // reduced Planck constant
const double SI_units::rho_crit_over_h2 = 2.77536627e11 * SI_units::Msol/pow(SI_units::Mpc,3); // critical density of the Universe

// Constants and specifications of the training simulations
const double SI_units::Lbox   = 1000 * SI_units::Mpc;
const double SI_units::Tgamma = 2.7255 * SI_units::Kelvin;
#endif
