# EuclidEmulator2 (version 1.0.1)
This repository contains the source code of EuclidEmulator2, a fast and accurate tool to estimate the non-linear correction to the matter power spectrum. 
In contrast to its predecessor EuclidEmulator, EuclidEmulator2 allows for 8-parameter cosmological models including massive neutrinos (assuming a degenerate hierarchy) and dynamical dark energy. EuclidEmulator2 is written in C++. For more information on EuclidEmulator please visit https://github.com/miknab/EuclidEmulator.

Author:   M. Knabenhans<br/>
Date of last update:      September 2020<br/>
Reference: Euclid Consortium: Knabenhans et al. (2020), (submitted to MNRAS)<br/>

If you use EuclidEmulator2 in any way (for a publication or otherwise), please cite this paper.

<b>Contact information:</b> If you have any questions and/or remarks related to this work, please do not hesitate to send me an email (mischakATphysik.uzh.ch)

## Currently implemented features
* emulation of the non-linear correction factor <i>B(k,z)</i>
* large allowed redshift interval: <i>z</i> in the interval [0.0,10.0] 
* spatial scales spanning more than three orders of magnitude: 8.73 x 10<sup>-3</sup> <i>h</i> / Mpc ≤ <i>k</i> ≤ 9.41 <i>h</i> / Mpc.

* C++ executable
* many different was to define the cosmologies:
  - direct definition of a single cosmology using command line parameters </li>
  - definition of several cosmologies through a parameter file </li>
  - definition of a cosmology through a CLASS or CAMB parameter file </li>
* results are written to output file

For a more extensive list of functionalities please the list of possible command line parameters shown [below](#building-and-installation).

## Features yet to be implemented
* resolution correction emulator
* allow customization of output <i>k</i> modes

## Quick start
### Prerequisites
In any case you need:
 * C++11 or later
 * GNU Scientific Library version 2.5 or higher (GSL; see https://www.gnu.org/software/gsl/)
 * g++ version 4.9.1 or higher
 * cython (for the python wrapper)
 
#### GSL install
On most machines, building GSL is relatively simple. To install it locally, e.g. in `~/local/gsl`, use
```
mkdir -p $HOME/local/gsl && cd $HOME/local
wget -c ftp://ftp.gnu.org/gnu/gsl/gsl-latest.tar.gz -O - | tar zxv
```
The install procedure follows standard steps, but each one might take several minutes. Execute each command separately and only continue if there are no errors.
```
./configure --prefix=$HOME/local/gsl
make
make check
make install
```
 Once done, make sure to add the GSL library to your library path with
 ```
 export LD_LIBRARY_PATH=$HOME/local/gsl/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
 ```
 
### Test installations
The code was successfully compiled on the following systems and environments:

* Mac OS X Mojave (10.14.6), with g++ version 9.3.0 and GSL version 2.6 (both g++ and GSL were installed with Homebrew)
* Linux (Red Hat 4.4.7-18), with g++ version 4.9.1 and GSL version 2.5
* Linux (Pop!_OS 20.04), with g++ version 9.3.0 and GSL version 2.6 (local GSL install)
 
### Get the code
If you have not done so already, either download this repository or clone it to your local host (under Linux you can get an unzipped tar-ball via
```
   wget -c https://github.com/miknab/EuclidEmulator2/archive/master.tar.gz -O - | tar zxv
```

### Building and installation
The current version of EuclidEmulator2 comes both as a command line interface (CLI) or a python wrapper.

#### CLI installation

In order to build the CLI, cd into the EuclidEmulator2 directory, check the `Makefile` and modify it as required (in particular the path of the `-I` and `-L` flag for GSL) and execute 

```
   make
```
This will create an executable file called `ee2.exe`. In order to check if the build was successful, execute

```
   ./ee2.exe --help
```

You should now see the following output:

```
Highly accurate and efficient AI-based predictor of the non-linear correction of the matter power spectrum.
Usage:
  EuclidEmulator2 [OPTION...]

  -b, --Omega_b arg    baryon density parameter
  -m, --Omega_m arg    (total) matter density parameter
  -s, --Sum_m_nu arg   sum of neutrino masses in eV
  -n, --n_s arg        spectral index
  -H, --hubble arg     dimensionless Hubble parameter
  -W, --w_0 arg        time-independent dark energy equation-of-state
                       parameter
  -w, --w_a arg        time-dependent dark energy equation-of-state parameter
  -A, --A_s arg        spectral amplitude
  -z, --redshift arg   redshift at which the NLC shall be computed
  -i, --inifile arg    path to CLASS or CAMB parameter file
  -p, --parfile arg    path to EE2 parameter file
  -o, --outfile arg    output file name (default: nlc)
  -d, --outdir arg     output directory (default: results)
  -v, --verbosity arg  verbosity level (0, 1 or 2) (default: 0)
  -h, --help           Print usage
```

#### python installation

To build the python wrapper simply execute

```
pip install .
```

To check if the installation was successful, just open python and do

```
import euclidemu2
```

A python notebook (test_euclid_emu2.ipynb) is also included, which includes an example for running the code and retrieving the boost factor.


### Sanity check
As a next step I recommend to go through the following:

  1. enter the EuclidEmulator2 directory (root level). Here you should see, amongst others, a `tests` directory.
  2. execute the command `mkdir results`
  3. execute the command `./tests/run_test.sh`
 
You should now see some output printed to screen that provides information about the cosmologies that are being treated by EuclidEmulator2. As a result, the following files should now be in the `results` directory:

```
  CAMB_example0.dat
  CLASS_example0.dat
  cmd_input_test.dat0.dat
  from_ee2_parfile0.dat
  from_ee2_parfile1.dat
  from_ee2_parfile2.dat
  from_ee2_parfile3.dat
```

In each file you should find a list of k modes (first column) and one or more B(k,z) column.

### Usage
There are several ways of calling EuclidEmulator2. The different options are briefly explained in the following:

* specify a cosmology directly by passing a value for each cosmological parameter (flags `-b, -m, -s, -n, -H, -W, -w, -A` and `-z` as well as there corresponding long versions). Notice that an arbitrary number of redshifts (`-z` flags) can be passed:

Example:
```
    ./ee2.exe -b 0.049 -m 0.319 -s 0.058 -n 0.96 -H 0.67 -W -1.0 -w 0.0 -A 2.1e-9\
              -z 0.0 -z 0.1081179 -z 0.4880429
```

* using the `-p` flag to pass an EuclidEmulator2 parameter file that contains at least one cosmology.

Example:
```
    ./ee2.exe -p tests/ee2_parfile.par
```

Checkout the file `tests/ee2_parfile.par` in order to learn more about how to structure such a parameter file.

* using the `-i` flag to pass a `CLASS` or `CAMB` parameter file.

Examples:
```
    ./ee2.exe -i tests/camb_parameter_file.ini -t CAMB
```
or
```
    ./ee2.exe -i tests/class_parameter_file.ini -t CLASS
```

Notice that the `-t` flag is mandatory in this case in order to tell the code whether it is reading a CLASS or a CAMB style \*.ini file.

## License
EuclidEmulator2 is free software, distributed under the GNU General Public License. This implies that you may freely distribute and copy the software. You may also modify it as you wish, and distribute these modified versions. You must always indicate prominently any changes you made in the original code and leave the copyright notices, and the no-warranty notice intact. Please read the General Public License for more details. 

