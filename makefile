default: all

all: miniopusdec

miniopusdec: main.cpp
	g++ -lopusfile -I/usr/include/opus main.cpp -o miniopusdec
