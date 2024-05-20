OUTPUTFILE=run
INSTALLDIR=./executables


.PHONY: all
all: $(OUTPUTFILE)
run: main.cpp
	g++ -O3 -std=c++20 -march=native -ffast-math -ftree-vectorize -flto -o run main.cpp -I ./include
