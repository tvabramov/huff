CXX=g++
CXXFLAGS=-I. -std=c++14
DEPS = worker.h
OBJ = worker.o huff.o 

all: huff

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

huff: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	
.PHONY: clean
	
clean:
	rm *.o huff
