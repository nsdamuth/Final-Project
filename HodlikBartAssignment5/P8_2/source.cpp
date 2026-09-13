/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242
** Assignment: 5
** File: 	source.cpp
** Description: A program that checks the spelling of all words in a file by
**              comparing them to a dictionary vector.
**
** Author: 	Bart Hodlik, Nick Damuth, Leonard Payne,
** Date: 	12Sep26
** -------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Function declarations
void loadDictionary(const string& filename, vector<string>& words);
bool isWordInDictionary(const string& word, const vector<string>& dictionary);
string cleanWord(const string& word);
void checkSpelling(const string& documentFilename, const vector<string>& dictionary);

// Author: Nick Damuth
int main() {
    vector<string> dictionary;
    string dictFile = "words.txt";
    string docFile = "document.txt";
    
    loadDictionary(dictFile, dictionary);
    
    // Sort dictionary for binary search (in case it is not already sorted)
    sort(dictionary.begin(), dictionary.end());
    
    cout << "Misspelled words in " << docFile << ":" << endl;
    checkSpelling(docFile, dictionary);
    
    return 0;
}

// Author: Bart Hodlik
void loadDictionary(const string& filename, vector<string>& words) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening dictionary file: " << filename << endl;
        return;
    }
    string word;
    while (inFile >> word) {
        words.push_back(word);
    }
    inFile.close();
}

// Author: Nick Damuth
bool isWordInDictionary(const string& word, const vector<string>& dictionary) {
    // Binary search for efficiency
    return binary_search(dictionary.begin(), dictionary.end(), word);
}

// Author: Leonard Payne
string cleanWord(const string& word) {
    string cleaned = "";
    for (char c : word) {
        if (isalpha(c)) {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

// Author: Leonard Payne
void checkSpelling(const string& documentFilename, const vector<string>& dictionary) {
    ifstream inFile(documentFilename);
    if (!inFile) {
        cerr << "Error opening document file: " << documentFilename << endl;
        return;
    }
    string word;
    while (inFile >> word) {
        string cleanedWord = cleanWord(word);
        if (!cleanedWord.empty()) {
            if (!isWordInDictionary(cleanedWord, dictionary)) {
                cout << cleanedWord << endl;
            }
        }
    }
    inFile.close();
}
