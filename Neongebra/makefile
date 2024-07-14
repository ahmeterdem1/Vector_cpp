CXXFLAGS := -Iinclude -Ineon_include -std=c++20 -Wall -g -march=arm64 -O3 -march=native -ffast-math -ftree-vectorize -flto
ALL_OBJECTS := bin/main.o bin/vectorgebra.o bin/graph.o bin/matrix.o bin/vector.o bin/complex.o \
			    	 bin/neonvector_int16.o bin/neonvector_int8.o bin/neonvector_uint8.o \
			    	 bin/functions.o bin/helpers.o bin/exceptions.o bin/node.o

EXEC := ./bin/main

bin/main: $(ALL_OBJECTS)
	g++ $(ALL_OBJECTS) $(CXXFLAGS) -o bin/main

bin/node.o: src/node.cpp include/*.h neon_include/*.h
	g++ -c src/node.cpp $(CXXFLAGS) -o bin/node.o

bin/exceptions.o: src/exceptions.cpp include/*.h neon_include/*.h
	g++ -c src/exceptions.cpp $(CXXFLAGS) -o bin/exceptions.o

bin/helpers.o: src/helpers.cpp include/*.h neon_include/*.h
	g++ -c src/helpers.cpp $(CXXFLAGS) -o bin/helpers.o

bin/functions.o: src/functions.cpp include/*.h neon_include/*.h
	g++ -c src/functions.cpp $(CXXFLAGS) -o bin/functions.o

bin/neonvector_int8.o: neon_src/neonvector_int8.cpp include/*.h neon_include/*.h
	g++ -c neon_src/neonvector_int8.cpp $(CXXFLAGS) -o bin/neonvector_int8.o

bin/neonvector_uint8.o: neon_src/neonvector_uint8.cpp include/*.h neon_include/*.h
	g++ -c neon_src/neonvector_uint8.cpp $(CXXFLAGS) -o bin/neonvector_uint8.o

bin/neonvector_int16.o: neon_src/neonvector_int16.cpp include/*.h neon_include/*.h
	g++ -c neon_src/neonvector_int16.cpp $(CXXFLAGS) -o bin/neonvector_int16.o

bin/complex.o: src/complex.cpp include/*.h neon_include/*.h
	g++ -c src/complex.cpp $(CXXFLAGS) -o bin/complex.o

bin/vector.o: src/vector.cpp include/*.h neon_include/*.h
	g++ -c src/vector.cpp $(CXXFLAGS) -o bin/vector.o

bin/matrix.o: src/matrix.cpp include/*.h neon_include/*.h
	g++ -c src/matrix.cpp $(CXXFLAGS) -o bin/matrix.o

bin/graph.o: src/graph.cpp include/*.h neon_include/*.h
	g++ -c src/graph.cpp $(CXXFLAGS) -o bin/graph.o

bin/vectorgebra.o: src/vectorgebra.cpp include/*.h neon_include/*.h
	g++ -c src/vectorgebra.cpp $(CXXFLAGS) -o bin/vectorgebra.o

bin/main.o: src/main.cpp include/*.h neon_include/*.h
	g++ -c src/main.cpp $(CXXFLAGS) -o bin/main.o

.PHONY: clean
clean:
	rm -f bin/*.o bin/main

.PHONY: run
run:
	./bin/main

.PHONY: inspect
inspect:
ifneq ($(wildcard $(EXEC)),)
	otool -tV bin/main > inspect.txt
else
	@echo "Executable does not exist, run make first."
endif
