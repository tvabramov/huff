#ifndef WORKER_H
#define WORKER_H

#include <vector>
#include <unordered_map>

struct TreeNode {
  TreeNode *left, *right;
  long long priority;
  char *val; // as optional;
};

std::unordered_map<char, long long> calcFreq(const std::vector<char>& data);

TreeNode* freq2HaffnamTree(const std::unordered_map<char, long long>& freq);

std::unordered_map<char, std::vector<bool>> haffnamTree2EncTable(TreeNode* head);

std::vector<bool> encode(const std::vector<char>& data, const std::unordered_map<char, std::vector<bool>>& table);

std::vector<char> decode(const std::vector<bool>& code, TreeNode* tree);

#endif  // WORKER_H