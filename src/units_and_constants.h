#ifndef UNITS_AND_CONSTANTS
#define UNITS_AND_CONSTANTS
#include <math.h>

namespace SI_units{
	// Base units
	const double second   = 1.0; // in seconds
	const double meter    = 1.0; // in meters
	const double kilogram = 1.0; // in kilogram
	const double Kelvin   = 1.0; // in Kelvin

	// derived units
	const double kilometer = 1000 * meter;
	const double Joule     = kilogram * pow(meter,2)/pow(second,2); // Joule
	const double Mpc       = 3.085677581282e22 * meter; // megaparsec
	const double Msol      = 1.98841e30 *kilogram; // solar kilogram

	// units constant
	const double c        = 2.99792458e8 * meter/second; // speed of light
	const double G        = 6.67428e-11 * pow(meter,3)/(kilogram*pow(second,2)); // Newton's constant of gravity
	const double kB       = 1.3806504e-23 * Joule/Kelvin; // Boltzmann constant
	const double eV       = 1.602176487e-19 * Joule; // electron Volt
	const double hbar     = 6.62606896e-34/(2*M_PI) * Joule * second; // reduced Planck constant
	const double rho_crit_over_h2 = 2.77536627e11 * Msol/pow(Mpc,3); // critical density of the Universe

	// Constants and specifications of the training simulations
	const double Lbox   = 1000 * Mpc;
	const double Tgamma = 2.7255 * Kelvin;
};

namespace planck_units{
	// Base units
	const double second   = 1.0/(5.391247e-44); // in planck times
	const double meter    = 1.0/(1.616255e-35); // in planck length
	const double kilogram = 1.0/(2.176434e-8);  // in planck mass
	const double Kelvin   = 1.0/(1.416785e32);  // in planck temperature

	// derived units
	const double Joule = kilogram * pow(meter,2)/pow(second,2); // in planckian energy units
	const double Mpc   = 3.085677581282e22 * meter; // in planck lengths
	const double Msol  = 1.98841e30 * kilogram; // solar mass in planck masses

	// natural constant
	const double c        = 1.0; // speed of light
	const double G        = 1.0; // Newton's constant of gravity
	const double kB       = 1.0; // Boltzmann constant
	const double eV       = 1.602176487e-19 * Joule; // electron Volt
	const double hbar     = 1.0; // reduced Planck constant
	const double rho_crit_over_h2 = 2.77536627e11 * Msol/pow(Mpc,3); // critical density of the Universe

	// Constants and specifications of the training simulations
	const double Lbox   = 1000 * Mpc;
	const double Tgamma = 2.7255 * Kelvin;
};

namespace pkdgrav_units{
    // Base units
    const double second   = 1.0/(5.391247e-44); // in planck times
    const double meter    = 1.0/(1.616255e-35); // in planck length
    const double kilogram = 1.0/(2.176434e-8);  // in planck mass
    const double Kelvin   = 1.0/(1.416785e32);  // in planck temperature

    // derived units
    const double Joule = kilogram * pow(meter,2)/pow(second,2); // in planckian energy units
    const double Mpc   = 3.085677581282e22 * meter; // in planck lengths
    const double Msol  = 1.98841e30 * kilogram; // solar mass in planck masses

    // natural constant
    const double c        = 1.0; // speed of light
    const double G        = 1.0; // Newton's constant of gravity
    const double kB       = 1.0; // Boltzmann constant
    const double eV       = 1.602176487e-19 * Joule; // electron Volt
    const double hbar     = 1.0; // reduced Planck constant
    const double rho_crit_over_h2 = 2.77536627e11 * Msol/pow(Mpc,3); // critical density of the Universe

    // Constants and specifications of the training simulations
    const double Lbox   = 1000 * Mpc;
    const double Tgamma = 2.7255 * Kelvin;
};

namespace cosmological_units{
    // Base units
    const double second   = 1.0/(5.391247e-44); // in planck times
    const double meter    = 1.0/(1.616255e-35); // in planck length
    const double kilogram = 1.0/(2.176434e-8);  // in planck mass
    const double Kelvin   = 1.0/(1.416785e32);  // in planck temperature

    // derived units
    const double Joule = kilogram * pow(meter,2)/pow(second,2); // in planckian energy units
    const double Mpc   = 3.085677581282e22 * meter; // in planck lengths
    const double Msol  = 1.98841e30 * kilogram; // solar mass in planck masses

    // natural constant
    const double c        = 1.0; // speed of light
    const double G        = 1.0; // Newton's constant of gravity
    const double kB       = 1.0; // Boltzmann constant
    const double eV       = 1.602176487e-19 * Joule; // electron Volt
    const double hbar     = 1.0; // reduced Planck constant
    const double rho_crit_over_h2 = 2.77536627e11 * Msol/pow(Mpc,3); // critical density of the Universe

    // Constants and specifications of the training simulations
    const double Lbox   = 1000 * Mpc;
    const double Tgamma = 2.7255 * Kelvin;
};

#endif
