//  //  prog4TweetGenerator.cpp
//     Generate somewhat random tweets based on a datafile.
//    AUTHOR :  MOHIT AGGARWAL, SAUMMAY SINGHAL(project Partner)
//    
//    *** We ended up submitting different codes. It will be really helpful if you grade the program separately for me as well as for him. ***
//    
//  CS 141 Spring 2019
//  Author: Dale Reed
//
//  Open up a data file and find a starting point for some set of consecutive words
//  and print the starting word.
//
//  Next find all occurrences in the datafile of those consecutive words, and collect the set
//  of words that immediately follow each of those occurrences.  Choose one of the words
//  in this newly created set and add it to the set of consecutive words, shifting them
//  over by one so the first word gets overwritten by the second, and so on.
//
//  Repeat this process until >=30 words are displayed and an end-of-sentence word is found,
//  or until a displayed word has a newline or a return character at the end of it.
//
#include <iostream>
#include <cctype>       // For the letter checking functions
#include <fstream>      // For file input
#include <vector>
#include <cstring>      // For strlen()
#include <cstdlib>      // For rand()
using namespace std;

//---------------------------------------------------------------------
void getInputWords(vector < string > & allWords) // List of all the words in the selected input file
{
   // Prompt for and get user choice for which data file to use
   cout << "Menu Options: \n" <<
      "  1. Use Trump tweets \n" <<
      "  2. Use Dalai Lama tweets \n" <<
      "  3. Use Mixed Trump / Dalai Lama tweets \n" <<
      "  4. Use small test file \n" <<
      "  5. Use tiny test file \n" <<
      "Your choice: ";
   int dataSelectionOption;
   cin >> dataSelectionOption;

   ifstream inputFileStream; // declare the input file stream
   // open input file and verify
   switch (dataSelectionOption) {
   case 1:
      inputFileStream.open("TrumpTweets.txt");
      break;
   case 2:
      inputFileStream.open("DalaiLamaTweets.txt");
      break;
   case 3:
      inputFileStream.open("TrumpLama.txt");
      break;
   case 4:
      inputFileStream.open("Test.txt");
      break;
   case 5:
      inputFileStream.open("tiny.txt");
      break;
   default:
      cout << "Invalid option chosen, exiting program. ";
      exit(-1);
      break;
   }
   if (!inputFileStream.is_open()) {
      cout << "Could not find input file.  Exiting..." << endl;
      exit(-1);
   }

   char c = ' ';
   char inputWord[81];

   // Read a character at a time from the input file, separating out individual words.
   inputWord[0] = '\0';
   int index = 0;
   while (inputFileStream.get(c)) {
      if (!isprint(c) && c != '\r' && c != '\n') {
         continue; // Skip non-printable characters and get the next one
      }
      if (c != ' ') {
         // If it is not a return or newline, add it to the string.
         // If it is a return or newline character, only add it if there
         //    are already other characters in the string.
         if ((c != '\r' && c != '\n') ||
            ((c == '\r' || c == '\n') && index > 0)
         ) {
            inputWord[index++] = c;
         }
      }
      // End the word when encountering a space or a return character.
      if (c == ' ' || c == '\r' || c == '\n') {
         // Null terminate the input word. Store it if its length is > 0 and it is printable.
         inputWord[index] = '\0';
         if (strlen(inputWord) > 0 && isprint(inputWord[0])) {
            allWords.push_back(string(inputWord));
         }
         // Check for special case where there is a space at the end of the line.  We don't want
         // to lose the end of line character ('\n' or '\r'), so we concatenate it to the
         // last word that was previously stored on the list.  First check to see if this is the case:
         if ((c == '\r' || c == '\n') && strlen(inputWord) == 0) {
            long indexOfLastWord = allWords.size() - 1;
            allWords.at(indexOfLastWord).append("\r");
         }

         index = 0; // Restart the index for the next word
      }
   } //end while( inputFileStream...)
}

//---------------------------------------------------------------------
void checkWords(vector < string > startWords, // List of first words in sentences
   vector < string > allWords, // All words from input file
   vector < string > wordList, // List of all words following search phrase
   string nextWord) // Nexts word found after search phrase
{
   int debugMenuOption = 0;

   do {
      // Prompt for and get user choice for which debug option to use
      cout << " \n" <<
         "    >>> Debug menu options: <<<\n" <<
         "        1. Display a start word \n" <<
         "        2. Display one of all words \n" <<
         "        3. Display wordWindow and next words \n" <<
         "        4. Exit debugging \n" <<
         "    Your choice -> ";
      cin >> debugMenuOption;

      int wordIndexValue = 0; // Used to read user input for debug options below

      switch (debugMenuOption) {
      case 1:
         cout << "    Enter an index value from 0 to " << startWords.size() - 1 << " to display a start word: ";
         cin >> wordIndexValue;
         cout << "    " << startWords.at(wordIndexValue) << endl;
         break;
      case 2:
         cout << "    Enter an index value from 0 to " << allWords.size() - 1 << " to display one of all words: ";
         cin >> wordIndexValue;
         cout << "    " << allWords.at(wordIndexValue) << endl;
         break;
      case 3:
         cout << "    WordWindow and next words are: ";
         for (int i = 0; i < wordList.size(); i++) {
            cout << wordList.at(i) << " ";
         }
         cout << nextWord << endl;
         break;
      } //end switch(...)
   } while (debugMenuOption != 4);

} //end debug()

// takes in input of words list and an empty vector to add all the starting words into that empty vector
void getStartWords(vector < string > & startWords, vector < string > allWords) {
   startWords.push_back(allWords.at(0));  // adding the first element
   string current;
   for (int x = 1; x < allWords.size() - 1; x++) {
      current = allWords.at(x);
      if ((current.at(current.length() - 1) == '\n' || current.at(current.length() - 1) == '\r')) {
         startWords.push_back(allWords.at(x + 1));

      }   // pushingback every first word that occurs after \n or \r
   }
}

//---------------------------------------------------------------------
int main() {
   vector < string > allWords; // List of all input words
   vector < string > startWords; // List of words that start sentences, to use
                                 //   in starting generated tweets.
    string nextWord;   // randomly generated word in the vector followWords // *** Abhinav: Use consistent indents
   int position;       // location of 1st random word generated

   cout << "Welcome to the tweet generator. " << endl << endl;
   getInputWords(allWords);
   
   getStartWords(startWords, allWords);   
   
   int wordWindowWidth = 0;    
   char userInput = ' ';

   // Main loop
   while (userInput != 'X') {
      cout << endl;
      cout << "Enter x to exit, or d/c to debug/continue followed by word window size: ";
      cin >> userInput;
      userInput = toupper(userInput); // fold user input into upper case

      if (userInput == 'X') {
         break; // Exit program
      }

      // Read in the wordWindowWidth to be used in both the "continue" and the "debug" modes
      cin >> wordWindowWidth;
      vector < string > wordList; // Stores the list of consecutive words that are searched for

      // Finding a random starting word in the startWords list
      int startingindex = rand() % startWords.size();
      string randomWord = startWords[startingindex];

      // Finding location of that starting random word
      for (int g = 0; g < allWords.size(); g++) {
         if (allWords[g] == randomWord) {
            position = g;
            break;
         }
      }
      // Storing the chosen number of consecutive starting words
      for (int r = position; r < position + wordWindowWidth; r++) {
         wordList.push_back(allWords[r]);
      }

      int wordCounter = 0;
      while (true) {
        // Making the follow Words vector and then generating a random word from the words inside it.
        // Calling the random word generated as next Word.
         vector < string > followWords;
         for (int c = 0; c < allWords.size() - wordWindowWidth; c++) {
            int counter1 = c; // *** Abhinav: Use more meaningful variable names
            int commonTimes = 0;
            string firstWord = wordList[0];                  
            if (allWords[c] == firstWord) {
               for (int f = 0; f < wordList.size(); f++) {
                  if (allWords[counter1] == wordList[f]) {
                     commonTimes += 1;
                     counter1++;

                  }
               }
            }
            if (commonTimes == wordWindowWidth) {                // checking if it is running only till the wordWindow size 
               followWords.push_back(allWords[c + wordWindowWidth]);  // pushing back the values if it is true
            }
         }

         int randomIndex;                 // Generating the random index to access the next word in allWords list
         if (followWords.size() >= 1) {
            randomIndex = (rand() % followWords.size());
            nextWord = followWords[randomIndex];
         }
          // If statement for entering the debug mode
         if (userInput == 'D') {
            checkWords(startWords, allWords, wordList, nextWord);
            break;
         }
         // Displaying the first word in the wordWindow
         cout << " " << wordList.at(0);
         wordCounter++;

         // conditions to end a tweet
         if (wordList[0].find("\n") == wordList[0].size() - 1) {
            break;
         }
         if (wordList[0].find("\r") == wordList[0].size() - 1) {
            break;
         }

         // Conditions to end a tweet by considering punctuations and no. of words 
         if ((wordCounter >= 30 && wordList[0].rfind("?") == wordList[0].size() - 1)) {
            break;
         }
         if ((wordCounter >= 30 && wordList[0].rfind(".") == wordList[0].size() - 1)) {
            break;
         }

         if ((wordCounter >= 30 && wordList[0].rfind("!") == wordList[0].size() - 1)) {
            break;
         }
         // Shifting words to "left" to prepare for next round, shifting nextWord into the "right" of the wordWindow words.
         for (int g = 0; g < wordList.size() - 1; g++) {
            wordList[g] = wordList[g + 1];
         }
         wordList[wordList.size() - 1] = nextWord;
      } // while ends( true)
      if (userInput == 'D')
         break;
   } //  while ends ( userInput != 'x')

   cout << endl << endl;

   return 0;
} //end main()