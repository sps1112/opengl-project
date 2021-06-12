#ifndef FILEREADER_H
#define FILEREADER_H

// Header Declarations
#include <utility/Utils.h>

#include <iostream>
#include <sstream>

// Length Functions

// Calculates length of character array
int GetCharArrayLength(char *charArray);
// Calculates length of const character array
int GetCharArrayLength(const char *charArray);

// Conversion Functions

// Convert const char array to char array
char *ConvertToCharArray(const char *charArray);
// Converts character Array to string
std::string ConvertToString(char *charArray);
// Converts chararcter array to float ("70" to 70.0)
float ConvertToFloat(char *charArray);

// Comparision Functions

// Check if 2 char arrays are the same
bool CompareCharArray(char *a, char *b);

// Extraction Functions

// Gets Single Line in a char array from starting Index till '\n'
char *GetLine(char *charList, int startIndex);
// Returns part of line from start to end index(excluded)
char *GetLinePart(char *line, int startIndex, int endIndex);

// Index Functions
// Gets Index of a Character in a Char Array
int GetCharIndex(char *charList, char targetChar);
// Gets Index of a Character from a Start Index
int GetOtherCharIndex(char *charList, int startIndex, char targetChar);
// Gets ending index of a line from a starting Index
int GetLineEndIndex(char *charList, int startIndex);
// Gets starting index of next line from a starting Index
int GetLineStartIndex(char *charList, int startIndex);
// Get starting Index of a char array in another char array
int GetStartIndexString(char *mainCharArray, char *targetCharArray);
// Get ending Index of a char array in another char array
int GetEndIndexString(char *mainCharArray, char *targetCharArray);
// Gets index of a line by skipping n lines from a startIndex
int SkipLines(char *charArray, int startIndex, int n);

#endif // !FILEREADER_H