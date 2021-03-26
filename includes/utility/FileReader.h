#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <sstream>

// Calculates length of character array
int GetCharArrayLength(char *charArray);
int GetCharArrayLength(const char *charArray);

char *ConvertToCharArray(const char *charArray);

// Converts character Array to string
std::string ConvertToString(char *charArray);
// Converts chararcter array to float ("70" to 70.0)
float ConvertToFloat(char *charArray);

bool CompareCharArray(char *a, char *b);

// Gets Single Line from a starting Index till '\n'
char *GetLine(char *charList, int startIndex);
// Returns part of line from start to end index(excluding the endIndex)
char *GetLinePart(char *line, int startIndex, int endIndex);

int GetCharIndex(char *charList, char targetChar);
int GetOtherCharIndex(char *charList, int startIndex, char targetChar);

// Gets ending index of a line from a starting Index
int GetLineEndIndex(char *charList, int startIndex);
// Gets starting index of next line from a starting Index
int GetLineStartIndex(char *charList, int startIndex);

// Get starting Index of a char array in another char array
int GetStartIndexString(char *mainCharArray, char *targetCharArray);
// Get ending Index of a char array in another char array
int GetEndIndexString(char *mainCharArray, char *targetCharArray);

int SkipLines(char *charArray, int startIndex, int lines);

#endif