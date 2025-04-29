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

bool TryPalindrome(const Trie& root, const Trie& current, const std::string& context, const std::string& infix, std::string::const_reverse_iterator begin, std::string::const_reverse_iterator end) {
  while (begin != end && *begin == ' ') {
    ++begin;
  }
  if (begin == end) {
    if (!current.final_) {
      return false;
    }
    std::string reversed(context);
    std::reverse(reversed.begin(), reversed.end());
    std::cout << context << infix << reversed << std::endl;
    return true;
  }
  auto it = current.next_.find(*begin);
  return it != current.next_.end() && TryPalindrome(root, it->second, context, infix, begin + 1, end) || current.final_ && infix.empty() && TryPalindrome(root, root, context, infix, begin, end);
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
    TryPalindrome(root, *current, context, "", context.crbegin(), context.crend());
    for (const auto& it : current->next_) {
      if (!context.empty() && context.back() != ' ') {
	TryPalindrome(root, it.second, context, {it.first}, context.crbegin(), context.crend());
      }
      context.push_back(it.first);
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
