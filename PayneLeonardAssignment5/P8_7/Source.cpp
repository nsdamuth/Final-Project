/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
** Class:      CSC 242
** Assignment: 5
** File:       Source.cpp
** Description: Encrypts or decrypts a text file using a random
**              monoalphabetic cipher and a keyword.
**
** Author:     Leonard Payne
** Date:       12Sep26
** -------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

int main(int argc, char* argv[])
{

    // Check that all required command-line arguments are entered
    if (argc < 5)
    {
        cout << "Not enough command-line arguments." << endl;
        return 1;
    }
    // Store the command-line arguments
    string mode = argv[1];
    string keyArgument = argv[2];
    string inputFilename = argv[3];
    string outputFilename = argv[4];
    // Make sure the key argument starts with -k
    if (keyArgument.length() < 3 || keyArgument.substr(0, 2) != "-k")
    {
        cout << "Invalid key argument." << endl;
        return 1;
    }
    // Remove -k from the key argument
    string key = keyArgument.substr(2);
    // Build the cipher alphabet starting with the keyword
    string cipher = "";
    // Add each letter from the key only once
    for (char c : key)
    {
        c = toupper(c);

        if (cipher.find(c) == string::npos)
        {
            cipher += c;
        }
    }
    // Add the remaining alphabet letters in reverse order
    for (char c = 'Z'; c >= 'A'; c--)
    {
        if (cipher.find(c) == string::npos)
        {
            cipher += c;
        }
    }
    // Make sure the user selected encrypt or decrypt mode
    if (mode != "-e" && mode != "-d")
    {
        cout << "Use -e to encrypt or -d to decrypt." << endl;
        return 1;
    }
	// Open the input and output files
    ifstream inputFile(inputFilename);
    ofstream outputFile(outputFilename);
    // Check that the input file opened successfully
    if (!inputFile)
    {
        cout << "Could not open input file." << endl;
        return 1;
    }
	// Check that the output file opened successfully
    if (!outputFile)
    {
        cout << "Could not open output file." << endl;
        return 1;
    }
    // Process the input file one character at a time
    char ch;

    while (inputFile.get(ch))
    {   // Encrypt or decrypt letters while leaving other characters unchanged
        if (isalpha(ch))
        {
            bool lowercase = islower(ch);
            char upper = toupper(ch);
            int position;
            char result;
            // Encrypt the letter using the cipher alphabet
            if (mode == "-e")
            {
                position = upper - 'A';
                result = cipher[position];
            }
            // Decrypt the letter back to the regular alphabet
            else
            {
                position = cipher.find(upper);
                result = 'A' + position;
            }
            // Keep the original letter's lowercase format
            if (lowercase)
            {
                result = tolower(result);
            }
            // Write the converted letter to the output file
            outputFile << result;
        }
        else
        {
            // Write non-letter characters without changing them
            outputFile << ch;
        }
    }
    // Cloose both files when processing is finished
    inputFile.close();
    outputFile.close();

    cout << "File processing complete." << endl;

    return 0;
}
    