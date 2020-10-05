#!/bin/bash
./ee2.exe -b 4.18613219e-2\
		  -m 2.88868201e-1\
		  -s 3.87617426E-2\
		  -n 9.94669748e-1\
		  -H 6.11313591e-1\
		  -W -7.07891438e-1\
		  -w -2.20396591e-1\
		  -A 2.43811423e-9\
		  -z 0.0 -z 0.1081179 -z 0.4880429 -z 1.010451 -z 1.998261 -z 3.035232 -z 5.255458 -z 10.0\
		  -o cmd_input_test.dat

./ee2.exe -p tests/ee2_parfile.par -o from_ee2_parfile

mkdir results/CAMBpath
./ee2.exe -i tests/CAMB_parfile.ini -d results/CAMBpath -t CAMB

mkdir results/CLASSpath
./ee2.exe -i tests/CLASS_parfile.ini -d results/CLASSpath -t CLASS
