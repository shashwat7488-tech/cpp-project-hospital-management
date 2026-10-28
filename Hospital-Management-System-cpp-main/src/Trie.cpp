#include "Trie.h"

using namespace std;

Trie::Trie() { root = make_shared<TrieNode>(); }

void Trie::insert(const string& word) {
    auto current = root;
    for (char c : word) {
        char lowerC = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        if (current->children.find(lowerC) == current->children.end()) current->children[lowerC] = make_shared<TrieNode>();
        current = current->children[lowerC];
    }
    current->isEndOfWord = true;
}

void Trie::collectAllWords(shared_ptr<TrieNode> node, string currentPrefix, vector<string>& results) const {
    if (node->isEndOfWord) results.push_back(currentPrefix);
    for (const auto& pair : node->children) collectAllWords(pair.second, currentPrefix + pair.first, results);
}

vector<string> Trie::searchPrefix(const string& prefix) const {
    vector<string> results;
    auto current = root;
    string lowerPrefix = "";
    for (char c : prefix) lowerPrefix += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    for (char c : lowerPrefix) {
        if (current->children.find(c) == current->children.end()) return results;
        current = current->children[c];
    }
    collectAllWords(current, lowerPrefix, results);
    return results;
}
