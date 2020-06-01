all:
	g++ src/main.cxx src/cosmo.cxx src/emulator.cxx src/parse.cxx -o ee2.exe -Isrc -Icxxopts/include -lgsl --std=c++11 
