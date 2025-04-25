#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void buildWords(string& guess, int pos, map<char, int>& floatMap, int floatLeft, const set<string>& dict, set<string>& results, const set<char>& allowedLetters);




// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> results; // set to store valid results
    string guess = in; // working string that gets filled

    //count floating letters
    map<char, int> floatMap;
    int floatLeft = 0;
    for (size_t i = 0; i < floating.size(); i++) {
      floatMap[floating[i]]++;
      floatLeft++;
    }

    // adjust floating map if some floating letters already in pattern
    int blanks = 0;
    for(size_t i = 0; i < guess.size(); i++) {
      char ch = guess[i];
      if (ch == '-') {
        blanks++; // count how many blanks that are left
      }
      else if (floatMap.find(ch) != floatMap.end()) {
        if (--floatMap[ch] == 0) { // if floating letter already present in pattern, remove one occurence of that letter from floating pool because already used in pattern
          floatMap.erase(ch);
        }
        floatLeft--;
      }
    }

    // build set of letters we can use to fill blanks
    set<char> allowedLetters(floating.begin(), floating.end());
    if (blanks > (int)floating.size()) {
      for (char c = 'a'; c <= 'z'; c++) { // if more blanks than floating, we can use other letters
        allowedLetters.insert(c);
      }
    }
    //  recursive backtracking from position 0
    buildWords(guess, 0, floatMap, floatLeft, dict, results, allowedLetters);
    return results; // return all valid words found
}

// Define any helper functions here
void buildWords(string& guess, int pos, map<char,int>& floatMap, int floatLeft, const set<string>& dict, set<string>& results, const set<char>& allowedLetters) {
  int len = guess.size(); // total length of target word

  // base case
  if (pos == len) {
    // if we filled all positions and used all floating letters
    if (floatMap.empty() && dict.count(guess)) {
      results.insert(guess); // only add if it is in dictionary
    }
    return;
  }

  // count remaining blanks
  int blanks = 0;
  for (int i = pos; i < len; i++) {
    if (guess[i] == '-') {
      blanks++;
    }
  }
  if (blanks < floatLeft) { // not enough blanks left to place remaining floating letters
      return;
  }


  // if already filled -> move onlyLetters: RECURSIVE STEP
  if (guess[pos] != '-') {
    // if current character is already fill -> go to next position
    buildWords(guess, pos + 1, floatMap, floatLeft, dict, results, allowedLetters);
  }
  else {
    // try placing each allowed letter in curr blank 
    for (set<char>::const_iterator it = allowedLetters.begin(); it != allowedLetters.end(); ++it) {
      char c = *it;
      guess[pos] = c; // place letter in guess

      map<char, int> nextFloatMap = floatMap; // copy of curr float map
      int nextFloatLeft = floatLeft;

      if (nextFloatMap.find(c) != nextFloatMap.end()) { 
        if (--nextFloatMap[c] == 0) {
          nextFloatMap.erase(c); // remove letter if count drops to 0
        }
        nextFloatLeft--; // one fewer floating letters left
      }
      // recurse to next pos
      buildWords(guess, pos + 1, nextFloatMap, nextFloatLeft, dict, results, allowedLetters);
      guess[pos] = '-'; // backtrack -> reset character for next loop iteration
    }
  }
}

