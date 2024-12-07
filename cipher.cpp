#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function prototypes at the top of the file for function decomposition

/**
 * Print instructions for using the program.
 */
void printMenu();
/**
 * Returns the 0-based index in the English alphabet where `c` appears,
 * or -1 if `c` is not an uppercase letter in the alphabet.
 *
 * For example:
 *  - `findIndexInAlphabet('A')` returns 0
 *  - `findIndexInAlphabet('D')` returns 3
 *  - `findIndexInAlphabet('+')` returns -1
 *  - `findIndexInAlphabet('a')` returns -1
 */
int findIndexInAlphabet(char c);
/**
 * Returns `c` rotated by `amount` many characters forward. If we run out
 * of letters in the alphabet, wrap around back to 'A'. This method
 * assumes that `c` is an uppercase letter.
 * For example:
 *  - `rot('A', 0)` returns 'A'
 *  - `rot('A', 1)` returns 'B'
 *  - `rot('Z', 1)` returns 'A'
 *  - `rot('A', 10)` returns 'K'
 *  - `rot('J', 25)` returns 'I'
 */
char rot(char c, int amount);
/**
 * Returns a new string in which every character has been rotated by `amount`
 * letters. Lowercase letters are converted to uppercase and rotated.
 * Non-alphabetic characters are left alone.
 * For example:
 *  `rot("A", 0)` returns "A"
 *  `rot("AA", 3)` returns "DD"
 *  `rot("HELLO", 0)` returns "HELLO"
 *  `rot("HELLO", 1)` returns "IFMMP"
 *  `rot("oh HEL-LO!", 1)` returns "PI IFM-MP!"
 */
string rot(string line, int amount);

void caesarCipher();
void vigenereCipher();
void decrypt( vector<string> dict );

int main() {
    string command; string word;
    ifstream inFS; vector<string> dict;

    inFS.open( "dictionary.txt" );
    if ( !inFS.is_open() ) {  cout << "**file not opened\n"; }
    while ( getline( inFS, word ) ) { dict.push_back( word ); }
    inFS.close();

    cout << "Welcome to Ciphers!" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    do {
        printMenu();
        cout << endl
             << "Enter a command (case does not matter): ";

        // Use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        // TODO_STUDENT: Execute non-exit commands
        if ( command == "C" || command == "c" ) {
          caesarCipher();
        } else if ( command == "V" || command == "v" ) {
          vigenereCipher();
        } else if ( command == "D" || command == "d" ) {
          decrypt( dict );
        } 
        cout << endl;
    } while (!(command == "x" || command == "X"));

    return 0;
}

void printMenu() {
    cout << "Ciphers Menu" << endl;
    cout << "------------" << endl;
    // Caesar Ciphers
    cout << "C - Encrypt with Caesar Cipher" << endl;
    cout << "D - Decrypt Caesar Cipher" << endl;
    // Vigenere
    cout << "V - Encrypt with Vigenère" << endl;
    // Utility
    cout << "X - Exit Program" << endl;
}

int findIndexInAlphabet(char c) { // DONE
  for ( int i = 0; i < ALPHABET.length(); i++ ) {
    if ( c == ALPHABET[i] ) { return i; }
  } return -1;
}

char rot(char c, int amount) { // DONE
  int i = findIndexInAlphabet(c);
  if ( i + amount > 25 ) { c = ALPHABET[i + amount - 26 ]; }
  else { c = ALPHABET[ i + amount ]; }
  return c;
}

string rot(string line, int amount) { // DONE
  for ( int i = 0; i < line.length(); i++ ) {
    if ( !isalpha( line.at(i) ) ) { continue; }
    if ( !isupper( line.at(i) ) ) {  line.at(i) = toupper( line.at(i)); }
    line.at(i) = rot( line.at(i), amount );
  }
  return line;
}

string cleanLine(string line) { 
  string cleanedLine = "";
  for ( int i = 0; i < line.length(); i++ ) {
    if ( isalpha( line.at(i) ) ) {
      if ( !isupper( line.at(i) ) ) { line.at(i) = toupper( line.at(i) ); }
      cleanedLine += line.at(i);
    }
  }
  return cleanedLine;
}

void caesarCipher() {
  cout << "Enter the text to encrypt:\n";
  string line; getline( cin, line );
  cout << "Enter the number of characters to rotate by:\n";
  string amount; getline( cin, amount );
  int amt = stoi( amount );
  cout << rot( line, amt ) << endl;
}

void vigenereCipher() {
  cout << "Enter the text to encrypt:\n";
  string line; getline( cin, line );
  cout << "Enter the Vigenere key:\n";
  string key; getline( cin, key );

  if ( key.length() < line.length() ) {
    for ( int i = 0; i < line.length() - key.length(); i++ ) {
        key += key[ i % key.length() ];
    }
  }

  for ( int i = 0; i < line.length(); i++ ) {
    if ( !isalpha( line.at(i) ) ) { continue; }
    if ( !isupper( line.at(i) ) ) {  line.at(i) = toupper( line.at(i) ); }
    line.at(i) = rot( line.at(i), findIndexInAlphabet( key.at(i % key.length()) ) );
  }
  cout << line << endl;
}

int find( vector<string> dict, string word ) {
  int hi = dict.size() - 1;
  int lo = 0;

  while ( lo <= hi ) {
    int mid = (hi + lo) / 2;
    if ( dict[ mid ] == word ) { return mid; }
    else if ( dict[ mid ] < word ) { lo = mid + 1; }
    else if ( dict[ mid ] > word ) { hi = mid - 1; }
  } return -1;
  // for ( int i = 0; i < dict.size(); i++ ) {
  //   if ( word == dict.at(i) ) { return i; }
  // } return -1;
}


int countMatches( vector<string> dict, vector<string> codeWords ) {
  int count = 0;
  for ( int i = 0; i < codeWords.size(); i++ ) {
    if ( find( dict, codeWords[i] ) != -1 ) { count++; }
  } return count;
}

void printDecryption( vector<string> codeWords ) {
//   cout << "printWords called\n";
  string decryption = "";
  for ( int i = 0; i < codeWords.size(); i++ ) {
    decryption += codeWords.at(i);
  }
  cout << decryption << endl;
}

bool validDecryption( vector<string> dict, vector<string> codeWords, int x ) {
  for ( int i = 0; i < codeWords.size(); i++ ) {
        codeWords.at(i) = rot( codeWords[i], x );
  }
  int matchCount = countMatches( dict, codeWords );

  if ( matchCount > codeWords.size() / 2 ) {
    printDecryption( codeWords );
    return true;
  }
  return false;
}

void decrypt( vector<string> dict ) {
  cout << "Enter the text to Caesar-cipher decrypt:\n";
  string code; getline( cin, code );
  vector<string> decryption;

  int x = code.find(' ');
  while ( x != string::npos ) {
    string temp = code.substr( 0, x );
    code.erase( 0, x + 1 );
    x = code.find(' ');
    decryption.push_back( temp );
  } 
  decryption.push_back( code );
  int printedLinesCount = 0;
  for ( int i = 0; i < 26; i++ ) {
    int validCode = validDecryption( dict, decryption, i );
    if ( validCode ) { printedLinesCount++; }
  }
  if ( printedLinesCount == 0 ) { cout << "No good decryptions found\n"; }
}