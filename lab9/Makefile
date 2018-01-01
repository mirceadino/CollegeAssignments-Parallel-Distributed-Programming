CXXFLAGS=-std=c++14

all: main_regular main_karatsuba generator

main_regular: main_regular.cpp polynomial.h utils.h
	mpic++ $(CXXFLAGS) main_regular.cpp -o main_regular

main_karatsuba: main_karatsuba.cpp polynomial.h utils.h
	mpic++ $(CXXFLAGS) main_karatsuba.cpp -o main_karatsuba

generator: generator.cpp
	g++ $(CXXFLAGS) generator.cpp -o generator

clean: all
	rm main_regular main_karatsuba generator
