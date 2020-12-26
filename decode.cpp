#include <iostream>

#include "huff.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "You are do it wrong. Usage: decode <input_file_name> "
            "<output_file_name>"
         << endl;
    return EXIT_FAILURE;
  }

  return huff::decode(argv[1], argv[2]) ? EXIT_SUCCESS : EXIT_FAILURE;
}
