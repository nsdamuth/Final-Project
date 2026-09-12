/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
** Class:       CSC 242: Introduction to Programming Concepts
** Assignment: Lesson 5
** File:        crypt.cpp
** Description: Random monoalphabet cipher
**
** Author:      Nicholas S. Damuth
** Date:        September 11, 2026
** -------------------------------------------------------------------------*/

// Includes
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;


// Build the cipher from the keyword
string get_cipher(string keyword) {

    string cipher = "";

    // First remove duplicate letters from the keyword
    for (size_t i = 0; i < keyword.length(); i++) {

        char letter = toupper(keyword[i]);

        // Only add letters that have not already been used
        if (isalpha(letter) && cipher.find(letter) == string::npos) {
            cipher += letter;
        }
    }

    // Append the remaining alphabet in reverse order
    for (char letter = 'Z'; letter >= 'A'; letter--) {

        if (cipher.find(letter) == string::npos) {
            cipher += letter;
        }
    }

    return cipher;
}


// Open the input file
ifstream get_file(string file_name) {

    ifstream input_file;
    input_file.open(file_name);

    return input_file;
}


// Encrypt one character
char encrypt_character(char character, string cipher) {

    // Anything that is not a letter does not need to change
    if (!isalpha(character)) {
        return character;
    }

    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    bool lower_case = islower(character);

    char upper_character = toupper(character);

    size_t position = alphabet.find(upper_character);

    char encrypted = cipher[position];

    // Preserve whether the original letter was lowercase
    if (lower_case) {
        encrypted = tolower(encrypted);
    }

    return encrypted;
}


// Decrypt one character
char decrypt_character(char character, string cipher) {

    // Anything that is not a letter does not need to change
    if (!isalpha(character)) {
        return character;
    }

    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    bool lower_case = islower(character);

    char upper_character = toupper(character);

    size_t position = cipher.find(upper_character);

    char decrypted = alphabet[position];

    // Preserve whether the original letter was lowercase
    if (lower_case) {
        decrypted = tolower(decrypted);
    }

    return decrypted;
}


// Main function
int main(int argc, char* argv[]) {

    string keyword = "";
    string input_name = "";
    string output_name = "";

    bool decrypt = false;

    // Read the command line arguments

    // following the sample provided of 
    // encrypt :crypt -kFEATHER encrypt.txt output.txt
    // decrypt : crypt -d -kFEATHER encrypt.txt output.txt
    for (int i = 1; i < argc; i++) {

        string argument = argv[i];

        // Determine if this is being decrypted
        if (argument == "-d") {
            decrypt = true;
        }

        // Fetch the keyword after -k
        else if (argument.length() > 2
                 && argument.substr(0, 2) == "-k") {

            keyword = argument.substr(2);
        }

        // First remaining argument becomes the input file
        else if (input_name.empty()) {
            input_name = argument;
        }

        // Second remaining argument becomes the output file
        else if (output_name.empty()) {
            output_name = argument;
        }
    }

    // A keyword is required by the assignment
    if (keyword.empty()) {
        cout << "Error: A keyword is required." << endl;
        return 1;
    }

    // Both files are required
    if (input_name.empty() || output_name.empty()) {
        cout << "Error: Input and output files are required." << endl;
        return 1;
    }

    // Build the cipher from the supplied keyword
    string cipher = get_cipher(keyword);

    // Open the input file
    ifstream input_file = get_file(input_name);

    // Guard
    if (!input_file) {
        cout << "Could not open input file." << endl;
        return 1;
    }

    // Open the output file
    ofstream output_file;
    output_file.open(output_name);

    // Guard
    if (!output_file) {
        cout << "Could not open output file." << endl;
        input_file.close();
        return 1;
    }

    // Bookend - Loop start
    char character;

    while (input_file.get(character)) {

        if (decrypt) {
            output_file << decrypt_character(character, cipher);
        }
        else {
            output_file << encrypt_character(character, cipher);
        }
    }

    // Bookend
    input_file.close();
    output_file.close();

    cout << "Completed file conversion." << endl;

    return 0;
}