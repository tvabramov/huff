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
	
  auto freq = calcFrequency(is);

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

  std::ofstream os("new.txt", ofstream::binary);
  is.seekg(0, is.beg);

  encode(is, os);

  is.close();
  os.close();

  std::ifstream is2("new.txt", ifstream::binary);
  std::ofstream os2("test2.txt", ofstream::binary);

  decode(is2, os2);

  is2.close();
  os2.close();
  /*std::vector<bool> encoded = encode(is, os, table);
  
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
  cout  << endl;*/

  return EXIT_SUCCESS;
}