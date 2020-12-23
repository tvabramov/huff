#include <map>
#include <algorithm>
#include <deque>
#include "worker.h"

#include <iostream>
using namespace std;

unordered_map<char, double> calcFrequency(ifstream& is) {
  if (!is)
    return {};
  is.seekg(0, is.beg);
  unordered_map<char, double> freq;
  char ch;
  while (is.read(&ch, sizeof(ch))) {
    cout << (int)ch << " ";
    freq[ch] += 1.0;
  }
  cout << endl;
  return freq;
}

// TODO: check with ugly tree (Fibonachi case).
TreeNode* freq2HaffnamTree(const unordered_map<char, double>& freq) {
  multimap<double, TreeNode*> m;
//cout << "freq size = " << freq.size() << endl;
  transform(freq.begin(), freq.end(), inserter(m, m.end()), [](const auto &p){
    TreeNode* node = new TreeNode(p.second /* priority */, p.first /* val */);
    return make_pair(p.second, std::move(node));
  });
//cout << "m size = " << m.size() << endl;
  
  while (m.size() > 1) {
    auto it1 = m.begin();
    auto it2 = next(m.begin());
    TreeNode* node = new TreeNode;
    node->left_ = it1->second;
    node->right_ = it2->second;
    node->val_ = nullptr;
    node->priority_ = it1->first + it2->first;
    //cout << "get pr = " << it1->first << " and " << it2->first << ", size = " << m.size() << endl;

    m.erase(it1);
    m.erase(it2);
    m.insert(make_pair(node->priority_, node));
  }

  // fictive node
  if (freq.size() == 1) {
    TreeNode* node = new TreeNode;
    node->left_ = m.begin()->second;
    node->right_ = nullptr;
    node->val_ = nullptr;
    node->priority_ = m.begin()->first + 1.0;

    return node;
  }

  return m.empty() ? nullptr : m.begin()->second;
}

void haffnamTree2EncTableCore(TreeNode* head, unordered_map<char, vector<bool>>& table, vector<bool> path) {
  if (head) {
    if (head->val_) {
      // DCHECK(!head->left && !head->right)
      table.insert(make_pair(*head->val_, path));
    } else {
      if (head->left_) {
        path.push_back(false);  // add 0;
        haffnamTree2EncTableCore(head->left_, table, path);
        path.pop_back();
      }
      if (head->right_) {
        path.push_back(true);  // add 0;
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

void encode(std::ifstream& is, std::ofstream& os) {
 // if (!is || !os) 
 //   return;
  is.clear();
  is.seekg(0, is.beg);
  auto freq = calcFrequency(is);

  TreeNode* tree = freq2HaffnamTree(freq);

  auto table = haffnamTree2EncTable(tree);

  {
    auto buf = freq.size();
    os.write(reinterpret_cast<const char *>(&buf), sizeof(buf));
    for (const auto &p : freq) {
      os.write(reinterpret_cast<const char *>(&p.first), sizeof(p.first));
      os.write(reinterpret_cast<const char *>(&p.second), sizeof(p.second));
    }
  }

  is.clear();
  is.seekg(0, is.beg);
  char ch;
  std::vector<bool> res;
cout << "CODE: ";
  while (is.read(&ch, sizeof(ch))) {
    auto it = table.find(ch);
    // DCHECK(it != table.end())
    auto code = it->second;
    copy(code.begin(), code.end(), back_inserter(res));
    // TODO: try at least word-size bulk writing.
    // TODO: refact this ugly code.
    if (res.size() >= sizeof(char) * 8) {
      char buf = 0;
      for (int i = 0; i < 8; ++i) {
        buf <<= 1;
        cout << res.front() ? "1" : "0";
        if (res.front())
          buf |= 1;
        res.erase(res.begin());
      }
      cout << " = " << (int)buf << " ";
      os.write(&buf, sizeof(buf));
    }
  }

  {
    char buf = res.size();
    os.write(&buf, sizeof(buf));
cout << "<size>";
    buf = 0;
    while (!res.empty()) {
      cout << res.front() ? "1" : "0";
      if (res.front())
        buf &= 1;
      buf << 1;
      res.erase(res.begin());
    }
    os.write(&buf, sizeof(buf));
  }

  cout << endl;
  os.flush();
}

void decode(std::ifstream& is, std::ofstream& os) {

  is.clear();
  is.seekg(0, is.beg);

  std::unordered_map<char, double> freq;
  {
    size_t table_size;
    if (!is.read(reinterpret_cast<char *>(&table_size), sizeof(table_size)))
      return;
    cout << "DECODE: table_size = " << table_size;

    char key;
    double val;
    for (auto i = 0; i < table_size; ++i) {
      if (!is.read(reinterpret_cast<char *>(&key), sizeof(key)))
        return;
      if (!is.read(reinterpret_cast<char *>(&val), sizeof(val)))
        return;
      freq.insert(make_pair(key, val));
    }
  }

  cout << endl << "DECODE:" << endl;
  for (const auto &p : freq) {
    cout << p.first << " = " << (int)p.first << ", count " << p.second << endl;
  }

  TreeNode* tree = freq2HaffnamTree(freq);

  deque<char> buffer;
  auto *node = tree;
  char ch;
  while (is.read(&ch, sizeof(ch))) {
    //cout << (int)ch << " ";
    buffer.push_back(ch);
    if (buffer.size() > 2) {
      ch = buffer.front();
      cout << (int)ch << " ";
      for (int i = 0; i < 8; ++i) {
        bool bit = ((ch & (1 << 7)) != 0) ? true : false;
        cout << bit ? "1" : "0";
        ch <<= 1;

        if (bit) {
          // DCHECK(node->right)
          node = node->right_;
        } else {
          // DCHECK(node->left)
          node = node->left_;
        }
        if (node->val_) {
          os.write(node->val_, sizeof(*node->val_));
          node = tree;
        }
      }
      cout << " ";
      buffer.pop_front();
    }
  }
  // DCKECK(buffer.size() == 2);
  char last_byte_size = buffer.front();
  char last_byte = buffer.back();
  for (int i = 0; i < last_byte_size; ++i) {
    bool bit = ((ch & (1 << 7)) != 0) ? true : false;
    cout << bit ? "1" : "0";
    ch <<= 1;

    if (bit) {
      // DCHECK(node->right)
      node = node->right_;
    } else {
      // DCHECK(node->left)
      node = node->left_;
    }
    if (node->val_) {
      os.write(node->val_, sizeof(*node->val_));
      node = tree;
    }
  }

  delete tree;
  /*std::vector<char> res;

  auto *node = tree;
  // DCHECK(!node->val)
  for (auto b: code) {
    if (b) {
      // DCHECK(node->right)
      node = node->right_;
    } else {
      // DCHECK(node->left)
      node = node->left_;
    }
    if (node->val_) {
      res.push_back(*node->val_);
      node = tree;
    }
  }

  return res;*/
}

/*
Тесты aaaaaaaaaaaaaaabbbbcc
      bbbbbbbbbbbbbbbaaaacc
      cccccccccccccccbbbbaa

для выявления ошибок, связанных с упорядочиванием и построением дерева.

Пустой вход
Односимвольный вход
Частоты Фибоначчи

*/