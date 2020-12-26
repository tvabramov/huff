CXX=g++
CXXFLAGS=-I. -std=c++14
DEPS = huff.h
OBJ_COMMON = huff.o
OBJ_ENCODE = encode.o
OBJ_DECODE = decode.o

all: encode decode

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

encode: $(OBJ_COMMON) $(OBJ_ENCODE)
	$(CXX) -o $@ $^ $(CXXFLAGS)

decode: $(OBJ_COMMON) $(OBJ_DECODE)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

test: all
	python test.py

clean:
	rm *.o encode decode
