#include <algorithm>
#include <cctype>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <map>

class Trie {
public:
  template <typename It>
  void Add(const It begin, const It end) {
    if (begin == end) {
      final_ = true;
      return;
    }
    next_[std::tolower(*begin)].Add(begin + 1, end);
  }

  bool final_ = false;
  std::map<char, Trie> next_;
};

bool TryPalindrome(const Trie& root, const Trie& current, std::string& context, bool infix, std::string::const_reverse_iterator begin, std::string::const_reverse_iterator end) {
  while (begin != end && *begin == ' ') {
    ++begin;
  }
  if (begin == end) {
    if (!current.final_) {
      return false;
    }
    std::cout << context << std::endl;
    return true;
  }
  auto it = current.next_.find(*begin);
  if (it != current.next_.end()) {
    context.push_back(*begin);
    bool found = TryPalindrome(root, it->second, context, infix, begin + 1, end);
    context.pop_back();
    if (found) {
      return true;
    }
  }
  if (!current.final_ || infix) {
    return false;
  }
  context.push_back(' ');
  bool found = TryPalindrome(root, root, context, infix, begin, end);
  context.pop_back();
  return found;
}

int main() {
  Trie root;
  std::ifstream words("/home/marcots/Downloads/br-sem-acentos.txt");
  std::string word;
  while (words >> word) {
    root.Add(word.cbegin(), word.cend());
  }

  std::string context;
  std::deque<std::pair<std::string, const Trie*>> todo{{"", &root}};
  while (!todo.empty()) {
    auto [context, current] = todo.front();
    todo.pop_front();
    TryPalindrome(root, *current, context, false, context.crbegin(), context.crend());
    for (const auto& it : current->next_) {
      context.push_back(it.first);
      if (context.size() > 1 && *(context.crbegin() + 1) != ' ') {
	TryPalindrome(root, it.second, context, true, context.crbegin() + 1, context.crend());
      }
      todo.push_back({context, &(it.second)});
      context.pop_back();
    }
    if (current->final_) {
      context.push_back(' ');
      todo.push_back({context, &root});
      context.pop_back();
    }
  }
  
  return 0;
}
