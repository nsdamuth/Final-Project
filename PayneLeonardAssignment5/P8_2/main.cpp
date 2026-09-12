/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
** Class:       CSC 242
** Assignment:  5
** File:        main.cpp
** Description: Checks words in a text file against a dictionary and prints
**              any words that are not found.
**
** Author:      Leonard Payne
** Date:        12Sep26
** -------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{   
    // Store the dictionary words and the current word b eing checked
    vector<string> words;
	string word;
    // Open the dictionary file
    ifstream dictionary("words");
	// Make sure the dictionary file opened successfully
    if (!dictionary)
    {
		cout << "Could not open the dictionary file." << endl;
		return 1;
    }
	// Read each dictionary word and add it to the vector
    while (dictionary >> word) {
        words.push_back(word);
    }
    // Close the dictionary file after loading all the words
    dictionary.close();
    // Open the text file that will be spell checked
	ifstream inputfile("input.txt");
	// Make sure the input file opened successfully
	if (!inputfile)
	{
        
		cout << "Could not open the input file." << endl;
		return 1;
	}
    // Display a heading before listing misspelled words
	cout << "Words not found in the dictionary:" << endl;
    // Reach each word from the input file
    while (inputfile >> word)
    {
        // Assume the word is not found until a match is found
        bool found = false;
        // Search through the dictionary words
        for (string dictWord : words)
        {
            // Check if the current word matches a dictionary word
            if (dictWord == word)
            {
                found = true;
                break;
            }
        }
        // Print the word if it was not found in the dictionary
        if (!found)
        {
            cout << word << endl;
        }
    }
    //CLose the input file
	inputfile.close();

    return 0;
}