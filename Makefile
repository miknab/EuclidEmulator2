# Make file for EuclidEmulator2
# =============================

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
