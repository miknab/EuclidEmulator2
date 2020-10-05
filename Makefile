# Make file for EuclidEmulator2
# =============================
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

# Compiler
# --------
CC        = g++ # requires at least g++ version 4.9.1

# Set include paths if necessary
# ------------------------------
# Notice that depending on the system and your setup 
# you may not necessarily have to specify the I_GSL variable.
# For instance, on my Mac OS X Mojave with g++ 9.3.0 it works
# without it.

I_CXXOPTS = -Icxxopts/include
I_GSL     = -I/opt/gsl/2.5/include # GSL versions older than 2.5 have not been tested.
								   # GSL versions < 2.0 will certainly not work.
INCL      = -Isrc $(I_CXXOPTS) $(I_GSL)

# Set libraries
# --------------
LIBPATH   = -L/opt/gsl/2.5/lib # path to GSL library
LIBS      = -lgsl -lgslcblas -lm

# Define all source files
# -----------------------
SRCS 	  = src/main.cxx src/cosmo.cxx src/emulator.cxx src/parse.cxx

# Define executable/output file name
# ----------------------------------
EXEC = -o ee2.exe

all:
	$(CC) $(SRCS) $(EXEC) $(INCL) $(LIBPATH) $(LIBS) --std=c++11

clean:
	rm -f *.o *.exe
