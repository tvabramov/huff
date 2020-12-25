#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "worker.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "You are do it wrong. Usage: encode <input_file_name> "
            "<output_file_name>"
         << endl;
    return EXIT_FAILURE;
  }

  ifstream is(argv[1] /* input_file_name */, ifstream::binary);
  if (!is) {
    cerr << "Cannot open input file: " << argv[0] << endl;
    return EXIT_FAILURE;
  }

  ofstream os(argv[2] /* output_file_name */, ofstream::binary);
  if (!is) {
    cerr << "Cannot open output file: " << argv[1] << endl;
    return EXIT_FAILURE;
  }

  encode(is, os);

  is.close();
  os.close();

  return EXIT_SUCCESS;
}