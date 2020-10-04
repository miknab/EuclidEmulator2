# Make file for EuclidEmulator2
# =============================

# Compiler
CC=g++

# Set include paths if necessary
I_CXXOPTS = -Icxxopts/include
#I_GSL    = -I/opt/gsl/2.5/include
INCL = -Isrc $(I_CXXOPTS) #$(I_GSL)

# Set libraries
LIBS     = -lgsl -lgslcblas -lm

# Define all source files
SRCS 	 = src/main.cxx src/cosmo.cxx src/emulator.cxx src/parse.cxx

# Define executable/output file name
EXEC = ee2.exe

all:
	$(CC) $(SRCS) -o $(EXEC) $(INCL) $(LIBS) --std=c++11
