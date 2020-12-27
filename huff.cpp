#include "huff.h"

#include <algorithm>
#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

namespace {

struct TreeNode {
  TreeNode *left_, *right_;
  long long priority_;
  char* val_;  // as optional;

  TreeNode() : left_(nullptr), right_(nullptr), priority_(0), val_(nullptr) {}
  explicit TreeNode(long long priority, char val)
      : left_(nullptr),
        right_(nullptr),
        priority_(priority),
        val_(new char{val}) {}
  explicit TreeNode(TreeNode* left, TreeNode* right, long long priority)
      : left_(left), right_(right), priority_(priority), val_(nullptr) {}

  ~TreeNode() {
    if (val_) delete val_;
    if (left_) delete left_;
    if (right_) delete right_;
  }
};

// TODO: deal with long long overflow
unordered_map<char, long long> calcFrequency(ifstream& is) {
  unordered_map<char, long long> freq;
  char ch;
  while (is.read(&ch, sizeof(ch))) {
    ++freq[ch];
  }
  return freq;
}

TreeNode* freq2HaffnamTree(const unordered_map<char, long long>& freq) {
  // Use pair <priority - char> as a key for unambiguous ordering of nodes (in
  // the case of same priority).
  multimap<pair<long long, char>, TreeNode*> m;
  transform(freq.begin(), freq.end(), inserter(m, m.end()), [](const auto& p) {
    TreeNode* node = new TreeNode(p.second /* priority */, p.first /* val */);
    return make_pair(make_pair(p.second, p.first), move(node));
  });

  while (m.size() > 1) {
    auto it1 = m.begin();
    auto it2 = next(m.begin());
    TreeNode* node =
        new TreeNode(it1->second /* left */, it2->second /* right */,
                     it1->first.first + it2->first.first /* priority */);
    m.erase(it1);
    m.erase(it2);
    m.insert(make_pair(make_pair(node->priority_, 0), node));
  }

  // fictive node.
  // TODO: explore its legitimacyю
  if (freq.size() == 1) {
    TreeNode* node =
        new TreeNode(m.begin()->second /* left */, nullptr /* right */,
                     m.begin()->first.first + 1 /* priority */);
    return node;
  }

  return m.empty() ? nullptr : m.begin()->second;
}

void haffnamTree2EncTableCore(TreeNode* head,
                              unordered_map<char, vector<bool>>& table,
                              vector<bool> path) {
  if (head) {
    if (head->val_) {
      assert(!head->left_ && !head->right_);
      table.insert(make_pair(*head->val_, path));
    } else {
      if (head->left_) {
        path.push_back(false);  // add 0;
        haffnamTree2EncTableCore(head->left_, table, path);
        path.pop_back();
      }
      if (head->right_) {
        path.push_back(true);  // add 1;
        haffnamTree2EncTableCore(head->right_, table, path);
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

}  // namespace

namespace huff {

bool encode(const char* ifn, const char* ofn) {
  ifstream is(ifn, ifstream::binary);
  if (!is) {
    cerr << "Cannot open input file: " << ifn << endl;
    return false;
  }

  ofstream os(ofn, ofstream::binary);
  if (!os) {
    cerr << "Cannot open output file: " << ofn << endl;
    return false;
  }

  auto freq = calcFrequency(is);

  TreeNode* tree = freq2HaffnamTree(freq);

  auto table = haffnamTree2EncTable(tree);

  delete tree;

  // Make the header = frequency table.
  // TODO: it would better to store the tree.
  {
    auto buf = freq.size();
    os.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
    for (const auto& p : freq) {
      os.write(reinterpret_cast<const char*>(&p.first), sizeof(p.first));
      os.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second));
    }
  }

  // TODO: optimize it.
  is.clear();
  is.seekg(0, is.beg);
  char ch;
  deque<bool> res;
  while (is.read(&ch, sizeof(ch))) {
    auto it = table.find(ch);
    assert(it != table.end());
    auto code = it->second;
    copy(code.begin(), code.end(), back_inserter(res));
    // TODO: try at least word-size bulk writing.
    // TODO: refact this ugly code.
    while (res.size() >= sizeof(char) * 8) {
      char buf = 0;
      for (int i = 0; i < 8; ++i) {
        buf <<= 1;
        if (res.front()) buf |= 1;
        res.pop_front();
      }
      os.write(&buf, sizeof(buf));
    }
  }

  {
    char buf = res.size();
    assert(buf < 8);
    os.write(&buf, sizeof(buf));
    buf = 0;
    for (int i = 0; i < 8; ++i) {
      buf <<= 1;
      if (!res.empty()) {
        if (res.front()) buf |= 1;
        res.pop_front();
      }
    }
    os.write(&buf, sizeof(buf));
  }

  os.close();

  return true;
}

bool decode(const char* ifn, const char* ofn) {
  ifstream is(ifn, ifstream::binary);
  if (!is) {
    cerr << "Cannot open input file: " << ifn << endl;
    return false;
  }

  ofstream os(ofn, ofstream::binary);
  if (!os) {
    cerr << "Cannot open output file: " << ofn << endl;
    return false;
  }

  unordered_map<char, long long> freq;
  {
    size_t table_size;
    if (!is.read(reinterpret_cast<char*>(&table_size), sizeof(table_size))) {
      cerr << "Cannot read encoded file header. It may be damaged.";
      return false;
    }

    char key;
    long long val;
    for (auto i = 0; i < table_size; ++i) {
      if (!is.read(reinterpret_cast<char*>(&key), sizeof(key)) ||
          !is.read(reinterpret_cast<char*>(&val), sizeof(val))) {
        cerr << "Cannot read encoded file header. It may be damaged.";
        return false;
      }
      freq.insert(make_pair(key, val));
    }
  }

  TreeNode* tree = freq2HaffnamTree(freq);

  deque<char> buffer;
  auto* node = tree;
  char ch;
  while (is.read(&ch, sizeof(ch))) {
    buffer.push_back(ch);
    if (buffer.size() > 2) {
      ch = buffer.front();
      for (int i = 0; i < 8; ++i) {
        bool bit = ((ch & (1 << 7)) != 0) ? true : false;
        ch <<= 1;

        if (bit) {
          assert(node->right_);
          node = node->right_;
        } else {
          assert(node->left_);
          node = node->left_;
        }
        if (node->val_) {
          os.write(node->val_, sizeof(*node->val_));
          node = tree;
        }
      }
      buffer.pop_front();
    }
  }
  assert(buffer.size() == 2);
  char last_byte_size = buffer.front();
  char last_byte = buffer.back();
  for (int i = 0; i < last_byte_size; ++i) {
    bool bit = ((last_byte & (1 << 7)) != 0) ? true : false;
    last_byte <<= 1;

    if (bit) {
      assert(node->right_);
      node = node->right_;
    } else {
      assert(node->left_);
      node = node->left_;
    }
    if (node->val_) {
      os.write(node->val_, sizeof(*node->val_));
      node = tree;
    }
  }

  delete tree;

  os.close();

  return true;
}

}  // namespace huff
