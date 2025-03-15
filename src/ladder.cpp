#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <cctype>
#include <set>
#include <vector>
#include <string>

using namespace std;

// error message
void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (\"" << word1 << "\" -> \"" << word2 << "\")\n";
}

// Check if the edit distance between str1 and str2 is <= d
bool edit_distance_within(const string& str1, const string& str2, int d) {
    // Quick check: if length difference is more than d, can't be within edit distance d
    if (abs((int)str1.size() - (int)str2.size()) > d) {
        return false;
    }

    // case 1: same length
    if (str1.size() == str2.size()) {
        int mismatchCount = 0;
        for (size_t i = 0; i < str1.size(); i++) {
            if (str1[i] != str2[i]) {
                mismatchCount++;
                if (mismatchCount > d) {
                    return false;
                }
            }
        }
        return true;
    }
    // case 2: lengths differ by exactly 1
    else {
        const string& longer  = (str1.size() > str2.size()) ? str1 : str2;
        const string& shorter = (str1.size() > str2.size()) ? str2 : str1;

        int i = 0, j = 0;
        int mismatchCount = 0;
        while (i < (int)longer.size() && j < (int)shorter.size()) {
            if (longer[i] != shorter[j]) {
                mismatchCount++;
                if (mismatchCount > d) {
                    return false;
                }
                i++;
            } else {
                i++;
                j++;
            }
        }
        if (i < (int)longer.size()) {
            mismatchCount++;
        }
        return (mismatchCount <= d);
    }
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

// Perform a BFS to find a shortest word ladder from begin_word to end_word
vector<string> generate_word_ladder(const string& begin_word,
                                    const string& end_word,
                                    const set<string>& word_list)
{
    // If start == end, no valid ladder per assignment instructions
    if (begin_word == end_word) {
        return {};
    }

    // Queue of partial ladders (each ladder is a vector of strings)
    queue<vector<string>> ladders;
    ladders.push({begin_word});

    // Keep track of words we have used so we don't revisit them
    set<string> visited;
    visited.insert(begin_word);

    while (!ladders.empty()) {
        vector<string> current_ladder = ladders.front();
        ladders.pop();
        string last_word = current_ladder.back();

        // Generate candidate words from last_word via substitution, insertion, and deletion
        set<string> candidates;

        // Substitution: change each character to every other letter
        for (size_t i = 0; i < last_word.size(); i++) {
            string candidate = last_word;
            for (char c = 'a'; c <= 'z'; c++) {
                if (candidate[i] == c) continue;
                candidate[i] = c;
                candidates.insert(candidate);
            }
        }

        // Insertion: insert a letter at every possible position
        for (size_t i = 0; i <= last_word.size(); i++) {
            for (char c = 'a'; c <= 'z'; c++) {
                string candidate = last_word.substr(0, i) + c + last_word.substr(i);
                candidates.insert(candidate);
            }
        }

        // Deletion: remove one character at every position
        if (!last_word.empty()) {
            for (size_t i = 0; i < last_word.size(); i++) {
                string candidate = last_word;
                candidate.erase(i, 1);
                candidates.insert(candidate);
            }
        }

        // Process each candidate: if candidate is in the dictionary and not visited, use it.
        for (const auto& cand : candidates) {
            if (visited.find(cand) == visited.end() && word_list.find(cand) != word_list.end()) {
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(cand);

                // If we've reached the target, return immediately
                if (cand == end_word) {
                    return new_ladder;
                }

                // Otherwise, enqueue and mark as visited
                ladders.push(new_ladder);
                visited.insert(cand);
            }
        }
    }
    // No ladder found
    return {};
}

// load all words from a file into a set
void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Could not open dictionary file: " << file_name << endl;
        return;
    }
    string word;
    while (in >> word) {
        for (auto &c : word) {
            c = tolower(static_cast<unsigned char>(c));
        }
        word_list.insert(word);
    }
    in.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << " ";
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    vector<string> ladder = generate_word_ladder("cat", "dog", word_list);
    print_word_ladder(ladder);
}
