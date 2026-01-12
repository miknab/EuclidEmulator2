/* emulator.h
*  ==========
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

#ifndef EMULATOR_H
#define EMULATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_errno.h>
#include <fstream>
#include <vector>
#include "cosmo.h"

#include <armadillo>

using namespace std;

class EuclidEmulator {
private:
  
  static constexpr int npcs = 14; //;
  static constexpr int nz = 101;  // number of redshifts in the training data
  static constexpr int nk = 613;  // number of k modes in training data
  static constexpr int n_coeffs[14] = {53, 53, 117, 117, 53, \
                                       117, 117, 117, 117, 521, \
                                       117, 1539, 173, 457};
  static constexpr int lmax = 16;
  static constexpr int nindices = 8;

  std::array<std::shared_ptr<gsl_interp2d>,npcs+1> logklogz2pc_spline; 

  /* Private data containers */
  arma::Mat<double>::fixed<nk*nz,npcs+1>        pc;                            // principal components, pc[:,0] = pc mean
  arma::Mat<double>::fixed<1539,npcs>           pce_coeffs;                    // PCE coefficients
  arma::Mat<int>::fixed<nindices*1539,npcs>     pce_multiindex;         // PCE multi-indices           
  arma::Col<double>::fixed<nk> logk;
  arma::Col<double>::fixed<nz> stp;

  /* Private member functions */
  void read_in_ee2_data_file();
  void pc_2d_interp();
  void print_info();

public:
  double kvec[nk];
  double Bvec[10*nz][nk];

  EuclidEmulator();
  ~EuclidEmulator() = default;
  void compute_nlc(Cosmology csm, vector<double> redshift);
  void write_nlc2file(const string &filename, vector<double> zvec, int n_redshift);
};

#endif
