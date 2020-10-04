SRCS = src/main.cxx src/cosmo.cxx src/emulator.cxx src/parse.cxx
INCLUDES = -Isrc -Icxxopts/include #-I/opt/gsl/2.5/include
LIBS = -lgsl -lgslcblas -lm

all:
	g++ $(SRCS) -o ee2.exe $(INCLUDES) $(LIBS) --std=c++11
