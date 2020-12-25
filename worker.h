#ifndef WORKER_H
#define WORKER_H

#include <fstream>
#include <unordered_map>
#include <vector>

struct TreeNode {
  TreeNode *left_, *right_;
  double priority_;
  char* val_;  // as optional;

  TreeNode(double priority, char val)
      : left_(nullptr),
        right_(nullptr),
        priority_(priority),
        val_(new char{val}) {}
  TreeNode() : left_(nullptr), right_(nullptr), priority_(0.0), val_(nullptr) {}

  // TODO: test it by drmemory
  ~TreeNode() {
    if (val_) delete val_;
    if (left_) delete left_;
    if (right_) delete right_;
  }
};

std::unordered_map<char, double> calcFrequency(std::ifstream& is);

TreeNode* freq2HaffnamTree(const std::unordered_map<char, double>& freq);

std::unordered_map<char, std::vector<bool>> haffnamTree2EncTable(
    TreeNode* head);

void encode(std::ifstream& is, std::ofstream& os);

void decode(std::ifstream& is, std::ofstream& os);

#endif  // WORKER_H