#include "ladder.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // If a dictionary file is provided on the command line, use it;
    // otherwise default to "words.txt"
    string dictionaryFile = (argc > 1) ? argv[1] : "words.txt";

    // Load the dictionary
    set<string> word_list;
    load_words(word_list, dictionaryFile);

    // Prompt the user for start and end words
    cout << "Enter the start word: ";
    string begin_word;
    cin >> begin_word;

    cout << "Enter the end word: ";
    string end_word;
    cin >> end_word;

    // Convert both words to lowercase (to match our dictionary)
    for (auto &c : begin_word) {
        c = tolower(static_cast<unsigned char>(c));
    }
    for (auto &c : end_word) {
        c = tolower(static_cast<unsigned char>(c));
    }

    // Check if start == end; if so, report an error or return
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return 0;
    }

    // Generate the word ladder
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);

    // Print the result
    print_word_ladder(ladder);

    return 0;
}
