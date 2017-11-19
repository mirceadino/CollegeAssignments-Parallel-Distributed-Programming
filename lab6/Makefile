CXXFLAGS=-std=c++14 -pthread

all: main generator

main: main.cpp polynomial.h
	g++ $(CXXFLAGS) main.cpp -o main

generator: generator.cpp
	g++ $(CXXFLAGS) generator.cpp -o generator

clean: main generator
	rm main generator 
