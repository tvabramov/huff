#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "worker.h"

using namespace std;

int main(int argc, char** argv)
{
  std::ifstream is("test.txt", ifstream::binary);
  if (!is) 
    return EXIT_FAILURE;
	
  // get length of file:
  is.seekg (0, is.end);
  int length = is.tellg();
  is.seekg (0, is.beg);
  
  std::cout << "Reading " << length << " characters... ";
  // read data as a block:
  std::vector<char> bytes(length);

  is.read(bytes.data(), length);

  is.close();

  for (int i = 0; i < length; ++i)
    cout << (int)bytes[i] << " ";
  cout << endl;
  
  auto freq = calcFreq(bytes);
  for (const auto &p : freq) {
    cout << p.first << " = " << (int)p.first << ", count " << p.second << endl;
  }

  TreeNode* tree = freq2HaffnamTree(freq);

  auto table = haffnamTree2EncTable(tree);
  
  cout << endl;
  for (const auto &p : table) {
    cout << p.first << " = " << (int)p.first << ", code ";
    for (auto bt : p.second)
      cout << bt;
    cout << endl;
  }

  std::vector<bool> encoded = encode(bytes, table);
  
  cout << "CODE = ";
  for (auto b : encoded) {
    cout << (b ? "1" : "0");
  }
  cout  << endl;

  std::vector<char> decoded = decode(encoded, tree);
  cout << "DECODE = ";
  for (auto ch : decoded) {
    cout << (int)ch << " ";
  }
  cout  << endl;

  return EXIT_SUCCESS;
}