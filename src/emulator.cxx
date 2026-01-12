/* emulator.cxx
*  =========
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

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h> // struct stat and fstat() function
#include <sys/mman.h> // mmap() function
#include <fcntl.h>    // declaration of O_RDONLY
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_sf_legendre.h>
#include <math.h>
#include <assert.h>
#include "emulator.h"

#include <stdio.h>
#include <limits.h>
#include <unistd.h>

using namespace std;

/* CONSTRUCTOR */
EuclidEmulator::EuclidEmulator()
{
  for(int i=0; i<this->npcs+1; i++) {
      this->logklogz2pc_spline[i] = NULL;
  }
  for(int iz = 0; iz<nz; iz++) {
    for(int ik = 0; ik<nk; ik++) {
      this->Bvec[iz][ik] = 0.0; // initialize the resulting nlc vector
    }
  }
  read_in_ee2_data_file();
  pc_2d_interp();

}

/* FUNCTION TO READ IN THE DATA FILE */
void EuclidEmulator::read_in_ee2_data_file()
{
  static off_t size;
  static struct stat s;
  static double *data = NULL;

  if (data == NULL) {
    std::cout << PATH_TO_EE2_DATA_FILE << std::endl;
    int fp = open(PATH_TO_EE2_DATA_FILE, O_RDONLY);
    if(!fp) {
      cerr << "Unable to open ./ee2_bindata.dat\n";
          exit(1);
    }
    int status = fstat(fp, & s);
    size = s.st_size;
    data = (double *) mmap (0, size, PROT_READ, MAP_PRIVATE, fp, 0);
  }

  int idx = 0;

  for (int i=0;i<this->npcs+1;i++) {
    for (int j=0; j<this->nk*this->nz; j++) {
      this->pc(j,i) = data[i*this->nk*this->nz + j];
    }
  }
  idx += (this->npcs+1)*this->nk*this->nz;
  
  for (int i=0;i<this->npcs;i++) {
    for (int j=0; j<this->n_coeffs[i]; j++){
      this->pce_coeffs(j,i) = data[idx + j];
    }
    idx += n_coeffs[i];
  }
  for (int i=0;i<this->npcs;i++) {
    for (int j=0; j<8*this->n_coeffs[i]; j++) {
      this->pce_multiindex(j,i) = data[idx + j];
    }
    idx += 8*this->n_coeffs[i];
  }
  for (int i=0;i<nk;i++) {
    this->kvec[i] = data[idx + i];  
  }
  idx += nk;

  assert(idx == size/sizeof(double));
}

void EuclidEmulator::pc_2d_interp()
{
  #pragma omp parallel for
  for (int i=0; i<this->nk; i++) {
    this->logk(i) = log(this->kvec[i]);
  }
  for (int i=nz-1; i>=0; i--) {
    this->stp(i) = i;
  }
  // VM: avoid interpolation boundary issues 
  // VM: (by not computing spline exactly at the border)
  this->logk(0) -= 1e-10;
  this->logk(this->nk-1) += 1e-10;
  this->stp(0) -= 1e-10;
  this->stp(this->nz-1) += 1e-10;

  for (int i=0; i<this->npcs+1; i++) {
    this->logklogz2pc_spline[i] = 
      std::shared_ptr<gsl_interp2d>(
          gsl_interp2d_alloc(gsl_interp2d_bilinear,this->nk,this->nz),
                             [](gsl_interp2d* p){gsl_interp2d_free(p);});
  }
  #pragma omp parallel for
  for (int i=0; i<this->npcs+1; i++) {
    gsl_interp2d_init(this->logklogz2pc_spline[i].get(), 
                      logk.memptr(), 
                      stp.memptr(), 
                      this->pc.colptr(i), 
                      this->nk, 
                      this->nz);
  }
}

void EuclidEmulator::compute_nlc(Cosmology csm, 
                                 std::vector<double> redshift)
{
  const int n_redshift= redshift.size();

  for(int iz=0; iz<n_redshift; iz++) {
    if(redshift.at(iz) > 10.0 || redshift.at(iz) < 0.0) {
      std::cout << "ERROR: EuclidEmulator2 accepts only redshifts in the interval [0.0, 10.0]\n" \
                << "The current redshift z = " << redshift.at(iz) << " is therefore ignored." << std::endl;
      continue;
    }
  }

  arma::Col<double> stp_no(n_redshift);
  #pragma omp parallel for
  for(int iz=0; iz<n_redshift; iz++) {
    stp_no(iz) = csm.compute_step_number(redshift.at(iz));
  }

  arma::Mat<double>::fixed<lmax+1,nindices>  univ_legendre;    // univariate legendre polynomials
  #pragma omp parallel for
  for (int ipar=0; ipar<this->nindices; ipar++) { // Pre-compute Legendre up to order lmax
    int status = gsl_sf_legendre_Pl_array(this->lmax, 
                                          csm.cosmo_tf[ipar], 
                                          univ_legendre.colptr(ipar));
    if (status) {
      cout << "error: " << gsl_strerror (status) << std::endl;
    }
    for (int l=0; l<=lmax; l++) {
      univ_legendre(l,ipar) *= sqrt(2.0*l + 1.0); //normalization
    }
  }

  arma::Col<double>::fixed<EuclidEmulator::npcs> pc_weight(arma::fill::zeros);
  for(int ipc=1; ipc<this->npcs+1; ipc++) {
    for(int ic=0; ic<n_coeffs[ipc-1]; ic++) {
      double basicfunc = 1.0;
      for(int ipar=0; ipar<this->nindices; ipar++) {
        basicfunc *= univ_legendre(this->pce_multiindex(ic*8+ipar,ipc-1),ipar);
      }
      pc_weight(ipc-1) += this->pce_coeffs(ic,ipc-1)*basicfunc;
    }
  }

  arma::Cube<double> tmp(n_redshift, nk, this->npcs+1);
  #pragma omp parallel for collapse(3)
  for(int iz=0; iz<n_redshift; iz++) {
    for(int ik=0; ik<nk; ik++) {
      for(int ipc=0; ipc<this->npcs+1; ipc++) {
        tmp(iz,ik,ipc) = gsl_interp2d_eval(logklogz2pc_spline[ipc].get(), 
                                           logk.memptr(), 
                                           stp.memptr(), 
                                           this->pc.colptr(ipc), 
                                           log(this->kvec[ik]), 
                                           stp_no(iz), 
                                           NULL, 
                                           NULL);
      }
    }
  }

  #pragma omp parallel for collapse(2)
  for(int iz=0; iz<n_redshift; iz++) {
    for(int ik=0; ik<nk; ik++) {
      Bvec[iz][ik] = tmp(iz,ik,0);
      for(int ipc=1; ipc<this->npcs+1; ipc++) {
        Bvec[iz][ik] += pc_weight(ipc-1)*tmp(iz,ik,ipc);
      }
    }
  }
}

void EuclidEmulator::write_nlc2file(const string& filename, vector<double> zvec, int n_redshift){
  ofstream fp_out (filename);
  // Writing an informative header line
  string header = "#k [h/Mpc]";
  for(int iz=0; iz<n_redshift; iz++){
    header.append("\tB(k,z="+to_string(zvec[iz])+")");
  }
  fp_out << header << endl;

  // Writing the data
  if (fp_out.is_open()) {
    for(int ik=0; ik<nk; ik++) {
      string line = to_string(this->kvec[ik]);
      for(int iz=0; iz<n_redshift; iz++) {
        line.append("\t");
          line.append(to_string(pow(10.0,Bvec[iz][ik])));
      }
      fp_out << line << endl;
    }
  }
  else cout << "Unable to open file" << endl;
}

void EuclidEmulator::print_info(){
  int i, j, ip, ic;

  for (i=1;i<14;++i) {
      fprintf(stderr,"%1d ",i);
      for (j=0;j<2*n_coeffs[i]-1;++j) fprintf(stderr,"-");
      fprintf(stderr,"\n");
      for (ic=0;ic<n_coeffs[i];++ic) {
          fprintf(stderr,"%.3g ",pce_coeffs(ic,i));
        }
      fprintf(stderr,"\n");
      for (j=0;j<2*n_coeffs[i]-1;++j) fprintf(stderr,"-");
      fprintf(stderr,"\n");
      for (ip=0;ip<8;++ip) {
          for (ic=0;ic<n_coeffs[i];++ic) {
            fprintf(stderr,"%1d ", pce_multiindex(ic*8 + ip,i));
          }
          fprintf(stderr,"\n");
      }
      fprintf(stderr,"\n");
    }
}
