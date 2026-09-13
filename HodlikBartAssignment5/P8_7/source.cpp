/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242
** Assignment: 5
** File: 	source.cpp
** Description: Random monoalphabet cipher encryption and decryption.
**
** Author: 	Bart Hodlik, Nick Damuth, Leonard Payne
** Date: 	12Sep26
** -------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Function declarations
string generateCipherAlphabet(const string& keyword);
void encryptFile(const string& inputFile, const string& outputFile, const string& cipherAlphabet);
void decryptFile(const string& inputFile, const string& outputFile, const string& cipherAlphabet);

// Author: Leonard Payne
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: crypt [-d|-e] -kKEYWORD input.txt output.txt" << endl;
        return 1;
    }

    bool decrypt = false;
    string keyword = "";
    string inputFile = "";
    string outputFile = "";

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-d") {
            decrypt = true;
        } else if (arg == "-e") {
            decrypt = false;
        } else if (arg.substr(0, 2) == "-k") {
            keyword = arg.substr(2);
        } else if (inputFile.empty()) {
            inputFile = arg;
        } else if (outputFile.empty()) {
            outputFile = arg;
        }
    }

    if (keyword.empty()) {
        cerr << "Error: Keyword not supplied." << endl;
        return 1;
    }

    string cipherAlphabet = generateCipherAlphabet(keyword);

    if (decrypt) {
        decryptFile(inputFile, outputFile, cipherAlphabet);
    } else {
        encryptFile(inputFile, outputFile, cipherAlphabet);
    }

    cout << "Operation completed successfully." << endl;
    return 0;
}

// Author: Bart Hodlik
string generateCipherAlphabet(const string& keyword) {
    string cipher = "";
    bool used[26] = {false};

    // Add unique letters from keyword
    for (char c : keyword) {
        if (isalpha(c)) {
            char upperC = toupper(c);
            int index = upperC - 'A';
            if (!used[index]) {
                cipher += upperC;
                used[index] = true;
            }
        }
    }

    // Append remaining letters in reverse order
    for (char c = 'Z'; c >= 'A'; --c) {
        int index = c - 'A';
        if (!used[index]) {
            cipher += c;
            used[index] = true;
        }
    }

    return cipher;
}

// Author: Nick Damuth
void encryptFile(const string& inputFile, const string& outputFile, const string& cipherAlphabet) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    if (!in || !out) {
        cerr << "Error opening files for encryption." << endl;
        return;
    }
    char c;
    while (in.get(c)) {
        if (isalpha(c)) {
            bool isLower = islower(c);
            char upperC = toupper(c);
            int index = upperC - 'A';
            char newChar = cipherAlphabet[index];
            out << (char)(isLower ? tolower(newChar) : newChar);
        } else {
            out << c;
        }
    }
    in.close();
    out.close();
}

// Author: Leonard Payne
void decryptFile(const string& inputFile, const string& outputFile, const string& cipherAlphabet) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    if (!in || !out) {
        cerr << "Error opening files for decryption." << endl;
        return;
    }
    char c;
    while (in.get(c)) {
        if (isalpha(c)) {
            bool isLower = islower(c);
            char upperC = toupper(c);
            int index = cipherAlphabet.find(upperC);
            char newChar = 'A' + index;
            out << (char)(isLower ? tolower(newChar) : newChar);
        } else {
            out << c;
        }
    }
    in.close();
    out.close();
}
