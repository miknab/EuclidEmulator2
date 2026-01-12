/* cosmo.h
*  =======
*  This file is part of EuclidEmulator2 
*  Copyright (c) 2020 Mischa Knabenhans
*
*  EuclidEmulator2 is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  EuclidEmulator2 is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COSMOLOGY_H
#define COSMOLOGY_H
#include <memory>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_spline.h>
#include <armadillo>

class Cosmology{
  public: 
    double cosmo[8], cosmo_tf[8];
    double Omega_gamma_0, Omega_nu_0, Omega_DE_0, rho_crit, T_gamma_0, T_nu_0;

    // Ranges for cosmological parameters 
    static constexpr double minima[8] = {0.04, 0.24, 0.00, 0.92, 0.61, -1.3, -0.7, 1.7e-9};
    static constexpr double maxima[8] = {0.06, 0.40, 0.15, 1.00, 0.73, -0.7, 0.7, 2.5e-9};
    
    /* Member functions */

    Cosmology(double Omega_b, double Omega_m, double Sum_m_nu, double n_s, double h, double w_0, double w_a, double A_s);
    ~Cosmology() = default;

    void read_from_file(char *filename);
    void print_cosmo();
    void print_cosmo_tf();
    double compute_step_number(double z); 
    static double rho_nu_i_integrand(double p, void * params);
    static double a2t_integrand(double a, void * params);

  private:
    static constexpr int nTable_  = 101;
    static constexpr double Neff  = 3.046;   
    
    arma::Col<double>::fixed<nTable_> avec;
    arma::Col<double>::fixed<nTable_> frac_nStep;

    double H0;

    typedef struct {
      double mnu_i;
      double a;
      Cosmology* csm_instance;
    } rho_nu_parameters;
    typedef struct {
      Cosmology* csm_instance;
    } a2t_parameters;

    // VM: LACK OF COPY CONSTRUCTOR IN THE ORIGINAL CODE CREATED A DOUBLE FREE ERROR 
    // VM: ON GSL* (DESTRUCTOR) AS PASSING COSMO BY VALUE COPIED THE GSL POINTER
    // VM: ORIGINAL AUTHOR JUST DELETED THE DESTRUCTOR CREATING A LEAK MEMORY
    // SOLUTION: SMART_PTR w/ custom deleter
    std::shared_ptr<gsl_interp> z2nStep_spline;
    std::shared_ptr<gsl_integration_glfixed_table> wglfixed_;

    /* Private member functions*/
    void check_parameter_ranges();

    double Hubble(double a);
    double Omega_matter(double a);
    double Omega_gamma(double a);
    double Omega_nu(double a);
    double Omega_DE(double a);
    double T_gamma(double a);
    double T_nu(double a);
    double a2t(double a);
    double a2dt(const double a, const double b);
    double a2Hubble(double a);
};
#endif
