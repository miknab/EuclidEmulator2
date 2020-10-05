# Test script for EuclidEmulator2
# ===============================
# This file is part of EuclidEmulator2 
# Copyright (c) 2020 Mischa Knabenhans
#
# EuclidEmulator2 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# EuclidEmulator2 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
		  -o cmd_input_test

./ee2.exe -p tests/ee2_parfile.par -o from_ee2_parfile
./ee2.exe -i tests/CAMB_parfile.ini -d results -t CAMB
./ee2.exe -i tests/CLASS_parfile.ini -d results -t CLASS
