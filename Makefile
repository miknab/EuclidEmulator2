all:
	g++ src/main.cxx src/cosmo.cxx src/emulator.cxx -o ee2.exe -Isrc -lgsl --std=c++11 
