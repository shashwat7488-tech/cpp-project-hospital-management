#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

class TrieNode {
public:
    unordered_map<char, shared_ptr<TrieNode>> children;
    bool isEndOfWord = false;
};

/**
 * @class Trie
 * @brief Prefix tree used as a Search Engine for fast O(L) autocomplete suggestions.
 */
class Trie {
private:
    shared_ptr<TrieNode> root;
    void collectAllWords(shared_ptr<TrieNode> node, string currentPrefix, vector<string>& results) const;

public:
    Trie();

    /** @brief O(L) Time (L=word length) | O(L * size) Space */
    void insert(const string& word);

    /** @brief O(L + K) Time (K=number of matches) | O(L + K) Space */
    vector<string> searchPrefix(const string& prefix) const;
};
