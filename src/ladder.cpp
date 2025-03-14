#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <cctype>

using namespace std;

// Simple helper for printing an error message
void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (\"" << word1 << "\" -> \"" << word2 << "\")\n";
}

// Check if the edit distance between str1 and str2 is <= d
// For this assignment, we only need d=1, but the code is written generally.
bool edit_distance_within(const string& str1, const string& str2, int d) {
    // Quick check: if length difference is more than d, can't be within edit distance d
    if (abs((int)str1.size() - (int)str2.size()) > d) {
        return false;
    }

    // Case 1: Same length
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
    // Case 2: Lengths differ by exactly 1
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
                // Skip a char in the longer string
                i++;
            } else {
                i++;
                j++;
            }
        }
        // If there's a leftover char in the longer string
        if (i < (int)longer.size()) {
            mismatchCount++;
        }
        return (mismatchCount <= d);
    }
}

// For this assignment, "adjacent" means edit distance <= 1
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

    // Standard BFS
    while (!ladders.empty()) {
        vector<string> current_ladder = ladders.front();
        ladders.pop();
        string last_word = current_ladder.back();

        // Check all dictionary words to see if they are neighbors
        for (auto& w : word_list) {
            if (visited.find(w) == visited.end() && is_adjacent(last_word, w)) {
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(w);

                // If we've reached the target, return immediately
                if (w == end_word) {
                    return new_ladder;
                }

                // Otherwise, enqueue and mark as visited
                ladders.push(new_ladder);
                visited.insert(w);
            }
        }
    }
    // No ladder found
    return {};
}

// Load all words from a file into a set (ignoring case by converting to lowercase)
void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Could not open dictionary file: " << file_name << endl;
        return;
    }
    string word;
    while (in >> word) {
        // Convert to lowercase for consistent comparisons
        for (auto &c : word) {
            c = tolower(static_cast<unsigned char>(c));
        }
        word_list.insert(word);
    }
    in.close();
}

// Print the ladder in a single line or indicate that no ladder was found
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    // Print each word separated by space (or " -> " if preferred)
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        cout << " ";
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    vector<string> generate_word_ladder("cat",   "dog",   word_list);
}
