/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242: Introduction to Programming Concepts
** Assignment: 	Lesson 5
** File: 	spell_check.cpp
** Description: final Project
**
** Author: 	Nicholas S. Damuth
** Date: 	September 11, 2026
** -------------------------------------------------------------------------*/

// Includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> get_dictionary() {
     // Define a vector of strings called words.
    vector<string> words;

    // Open the dictionary file
    ifstream dictionary;
    dictionary.open("/usr/share/dict/words");

    if (!dictionary) {
        cout << "Could not open dictionary file." << endl;
        return words;
    }

    string word;

    // For each word in the dictionary file

    // Append the word to the words vector.
    while (dictionary >> word) {
        words.push_back(word);
    }

    // closing handler
    dictionary.close();
    return words;
}

ifstream get_file(string file_name) {
    if (file_name.empty()) {
        file_name = "final.txt";
    }
    ifstream input_file;
    input_file.open(file_name);
    return input_file;
}

// Main function
int main() {

    // fetch the dictionary vector
    vector<string> words = get_dictionary();
    if (words.empty()) {
        cout << "Dictionary is empty" << endl;
        return 1;
    }

    // Abstracting as a function and param for possible future proofing of abstracting for file inputs
    ifstream input_file = get_file("");

    if (!input_file) {
        cout << "Could not open file to check." << endl;
        return 1;
    }
    // Bookend - Loop start
    string check_word;
    int errors = 0;
    while (input_file >> check_word) {

        bool found = false;

        // Search dictionary vector for the word
        
        for (size_t i = 0; i < words.size(); i++) {

            if (check_word == words[i]) {
                found = true;
                break;
            }
        }

        if (!found) {
            cout << check_word << endl;
            errors++;
        }
    }
    // Bookend
    input_file.close();
    cout << "Completed Spell check with " << errors << " errors." << endl;
    return 0;
}