#ifndef FILEREADER_H
#define FILEREADER_H

// Header Declarations
#include <utility/Utils.h>

// Standard Headers
#include <iostream>
#include <sstream>

// Length Functions

// Calculates length of character array
int get_char_array_length(char *charArray);
// Calculates length of const character array
int get_char_array_length(const char *charArray);

// Conversion Functions

// Convert const char array to char array
char *convert_to_char_array(const char *charArray);
// Converts character Array to string
std::string convert_to_string(char *charArray);
// Converts chararcter array to float ("70" to 70.0)
float convert_to_float(char *charArray);

// Comparision Functions

// Check if 2 char arrays are the same
bool compare_with_char_array(char *a, char *b);

// Extraction Functions

// Gets Single Line in a char array from starting Index till '\n'
char *get_line(char *charList, int startIndex);
// Returns part of line from start to end index(excluded)
char *get_line_part(char *line, int startIndex, int endIndex);

// Index Functions

// Gets Index of a Character in a Char Array
int get_char_index(char *charList, char targetChar);
// Gets Index of a Character from a Start Index
int get_other_char_index(char *charList, int startIndex, char targetChar);
// Gets ending index of a line from a starting Index
int get_line_end_index(char *charList, int startIndex);
// Gets starting index of next line from a starting Index
int get_line_start_index(char *charList, int startIndex);
// Get starting Index of a char array in another char array
int get_start_index_string(char *mainCharArray, char *targetCharArray);
// Get ending Index of a char array in another char array
int get_end_index_string(char *mainCharArray, char *targetCharArray);
// Gets index of a line by skipping n lines from a startIndex
int skip_lines(char *charArray, int startIndex, int n);

#endif // !FILEREADER_H
