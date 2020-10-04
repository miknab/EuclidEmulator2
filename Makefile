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
I_GSL    = -I/opt/gsl/2.5/include # requires at least GSL version 2.0 (tested only with 2.5 and 2.6)
INCL      = -Isrc $(I_CXXOPTS) $(I_GSL)

# Set libraries
# --------------
LIBPATH   = -L/opt/gsl/2.5/lib
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
