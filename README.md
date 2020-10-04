# EuclidEmulator2 (version 2.0)
This repository contains the source code of EuclidEmulator2, a fast and accurate tool to estimate the non-linear correction to the matter power spectrum. 
In contrast to its predecessor EuclidEmulator, EuclidEmulator2 allows for 8-parameter cosmological models including massive neutrinos (assuming a degenerate hierarchy) and dynamical dark energy. EuclidEmulator2 is written in C++. For more information on EuclidEmulator please visit https://github.com/miknab/EuclidEmulator.

Author:   M. Knabenhans<br/>
Date of last update:      September 2020<br/>
Reference: Euclid Consortium: Knabenhans et al. (2020), (;submitted to MNRAS)<br/>

If you use EuclidEmulator2 in any way (for a publication or otherwise), please cite this paper.

## Currently implemented features
* emulation of the non-linear correction factor <i>B(k,z)</i>
* large allowd redshift interval: <i>z</i> in the interval [0.0,10.0] 
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
* pip-installable python3 wrapper

## Contact information
If you have any questions and/or remarks related to this work, please do not hesitate to send me an email (mischakATphysik.uzh.ch)

## Quick start
### Prerequisites
In any case you need:
 * C++11 or later
 * GNU Scientific Library version 2.5 or higher (GSL; see https://www.gnu.org/software/gsl/)
 * g++ version 4.9.1 or higher
 
### Test installations
The code was successfully compiled on the following systems and environments:

* Mac OS X Mojave (10.14.6), with g++ version 9.3.0 and GSL version 2.6 (both g++ and GSL were installed with Homebrew)
* Linux (Red Hat 4.4.7-18), with g++ version 4.9.1 and GSL version 2.5

 
### Get the code
If you have not done so already, either download this repository or clone it to your local host (under Linux you can get a gzipped tar-ball via
```
   wget https://github.com/miknab/EuclidEmulator2/archive/master.tar.gz
```

### Building and installation
The current version of EuclidEmulator2 comes as a command line interface (CLI). In order to compile it, cd into the EuclidEmulator2 directory, modify the `Makefile` if you want to and execute 

```
   make
```
This will create an executable file called `ee2.exe`. In order to check if the build was successful, execute

```
   ./ee2.exe --help
```

You should now see the following output:
<a 

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

### Usage
There are several ways of calling EuclidEmulator2. The different options are briefly explained in the following:

* specify a cosmology directly by passing a value for each cosmological parameter (flags `-b, -m, -s, -n, -H, -W, -w, -A` and `-z` as well as there corresponding long versions). Notice that an arbitrary number of redshifts (`-z flags) can be passed:

Example:
```
    ./ee2.exe -b 0.049 -m 0.319 -s 0.058 -n 0.96 -H 0.67 -W -1.0 -w 0.0 -A 2.1e-9\
              -z 0.0 -z 0.1081179 -z 0.4880429
```

* using the `-p` flagpass an input file that contains at least one cosmology.

Example:
```
    ./ee2.exe -p tests/test_csm.dat
```

* using the `-i` flag to pass a `CLASS` or `CAMB` parameter file.

Examples:
```
    ./ee2.exe -i tests/camb_parameter_file.ini
```
or
```
    ./ee2.exe -i tests/class_parameter_file.ini
```



## License
EuclidEmulator2 is free software, distributed under the GNU General Public License. This implies that you may freely distribute and copy the software. You may also modify it as you wish, and distribute these modified versions. You must always indicate prominently any changes you made in the original code and leave the copyright notices, and the no-warranty notice intact. Please read the General Public License for more details. 

