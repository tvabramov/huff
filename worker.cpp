#include <map>
#include <algorithm>
#include "worker.h"

#include <iostream>
using namespace std;

unordered_map<char, long long> calcFreq(const std::vector<char>& data) {
  unordered_map<char, long long> out;
  for (const auto &el : data) {
    ++out[el];
  }
  
  return out;
}

TreeNode* freq2HaffnamTree(const std::unordered_map<char, long long>& freq) {
  multimap<long long, TreeNode*> m;
//cout << "freq size = " << freq.size() << endl;
  transform(freq.begin(), freq.end(), std::inserter(m, m.end()), [](const auto &p){
    TreeNode* node = new TreeNode;
    node->left = nullptr;
    node->right = nullptr;
    node->val = new char{p.first};
    node->priority = p.second;
    return make_pair(node->priority, node);
  });
//cout << "m size = " << m.size() << endl;
  
  while (m.size() > 1) {
    auto it1 = m.begin();
    auto it2 = next(m.begin());
    TreeNode* node = new TreeNode;
    node->left = it1->second;
    node->right = it2->second;
    node->val = nullptr;
    node->priority = it1->first + it2->first;
    //cout << "get pr = " << it1->first << " and " << it2->first << ", size = " << m.size() << endl;

    m.erase(it1);
    m.erase(it2);
    m.insert(make_pair(node->priority, node));
  }
  return m.empty() ? nullptr : m.begin()->second;
}

void haffnamTree2EncTableCore(TreeNode* head, unordered_map<char, vector<bool>>& table, vector<bool> path) {
  if (head) {
    if (head->val) {
      // DCHECK(!head->left && !head->right)
      table.insert(make_pair(*head->val, path));
    } else {
      if (head->left) {
        path.push_back(false);  // add 0;
        haffnamTree2EncTableCore(head->left, table, path);
        path.pop_back();
      }
      if (head->right) {
        path.push_back(true);  // add 0;
        haffnamTree2EncTableCore(head->right, table, path);
        path.pop_back();
      }
    }
  }
}

unordered_map<char, vector<bool>> haffnamTree2EncTable(TreeNode* head) {
  unordered_map<char, vector<bool>> table;
  vector<bool> buf;
  haffnamTree2EncTableCore(head, table, buf);
  return table;
}

vector<bool> encode(const vector<char>& data, const unordered_map<char, vector<bool>>& table) {
  vector<bool> res;

  for (const auto& ch : data) {
    auto it = table.find(ch);
    // DCHECK(it != table.end()
    auto code = it->second;
    copy(code.begin(), code.end(), back_inserter(res));
  }

  return res;
}

std::vector<char> decode(const std::vector<bool>& code, TreeNode* tree) {
  std::vector<char> res;

  auto *node = tree;
  // DCHECK(!node->val)
  for (auto b: code) {
    if (b) {
      // DCHECK(node->right)
      node = node->right;
    } else {
      // DCHECK(node->left)
      node = node->left;
    }
    if (node->val) {
      res.push_back(*node->val);
      node = tree;
    }
  }

  return res;
}

/*
Тесты aaaaaaaaaaaaaaabbbbcc
      bbbbbbbbbbbbbbbaaaacc
      cccccccccccccccbbbbaa

для выявления ошибок, связанных с упорядочиванием и построением дерева.
*/