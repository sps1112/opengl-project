#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <sstream>

// Calculates length of character array
int GetCharArrayLength(const char *charArray)
{
    int index = 0;
    while ((char)charArray[index] != '\0')
    {
        index++;
    }
    return (index + 1);
}

int GetCharArrayLength(char *charArray)
{
    int index = 0;
    while ((char)charArray[index] != '\0')
    {
        index++;
    }
    return (index + 1);
}

// Converts character Array to string
std::string ConvertToString(const char *charArray)
{
    std::string s = "";
    for (int i = 0; i < GetCharArrayLength(charArray); i++)
    {
        s = s + charArray[i];
    }
    return s;
}

std::string ConvertToString(char *charArray)
{
    std::string s = "";
    for (int i = 0; i < GetCharArrayLength(charArray); i++)
    {
        s = s + charArray[i];
    }
    return s;
}

// Converts chararcter array to float ("70" to 70.0)
float ConvertToFloat(const char *charArray)
{
    std::string s = ConvertToString(charArray);
    std::stringstream ss(s);
    float value{0};
    ss >> value;
    return value;
}

// Gets Single Line from a starting Index till '\n'
char *GetLine(const char *charList, int startIndex)
{
    char *newLine = new char[GetCharArrayLength(charList)];
    int index = startIndex;
    while ((char)charList[index] != '\n')
    {
        newLine[index - startIndex] = charList[index];
        index++;
    }
    newLine[index - startIndex] = '\0';
    return newLine;
}

char *GetLine(char *charList, int startIndex)
{
    char *newLine = new char[GetCharArrayLength(charList)];
    int index = startIndex;
    int count = 0;
    while ((char)charList[index] != '\n' && (char)charList[index] != '\0')
    {
        newLine[index - startIndex] = charList[index];
        index++;
    }
    newLine[index - startIndex] = '\0';
    return newLine;
}

// Gets ending index of a line from a starting Index
int GetLineEndIndex(const char *charList, int startIndex)
{
    int index = startIndex;
    while ((char)charList[index] != '\n')
    {
        index++;
    }
    return index;
}

int GetLineEndIndex(char *charList, int startIndex)
{
    int index = startIndex;
    while ((char)charList[index] != '\n')
    {
        index++;
    }
    return index;
}

// Gets starting index of next line from a starting Index
int GetLineStartIndex(const char *charList, int startIndex)
{
    return (GetLineEndIndex(charList, startIndex) + 1);
}

int GetLineStartIndex(char *charList, int startIndex)
{
    return (GetLineEndIndex(charList, startIndex) + 1);
}

// Get starting Index of a char array in another char array
int GetStartIndexString(char *mainCharArray, const char *targetCharArray)
{
    int mainLength = GetCharArrayLength(mainCharArray);
    int targetLength = GetCharArrayLength(targetCharArray);
    if (mainLength >= targetLength)
    {
        std::cout << "Inside if body" << std::endl;
        int index = 0;
        bool isChecking = false;
        bool hasIndex = false;
        int stringIndex = -1;
        int targetIndex = 0;
        while ((char)mainCharArray[index] != '\0')
        {
            if (isChecking)
            {
                if (targetIndex < targetLength - 1)
                {
                    if (mainCharArray[index] != targetCharArray[targetIndex])
                    {
                        isChecking = false;
                    }
                }
                else
                {
                    hasIndex = true;
                    stringIndex = index - (targetLength - 1);
                    break;
                }
            }
            if (!isChecking)
            {
                if (mainCharArray[index] == targetCharArray[0])
                {
                    std::cout << "found" << std::endl;
                    isChecking = true;
                    targetIndex = 0;
                }
            }
            index++;
            if (isChecking)
            {
                targetIndex++;
            }
        }
        return stringIndex;
    }
    std::cout << "INVALID TARGET STRING" << std::endl;
    return -1;
}

// Get starting Index of a char array in another char array
int GetStartIndexString(const char *mainCharArray, const char *targetCharArray)
{
    int mainLength = GetCharArrayLength(mainCharArray);
    int targetLength = GetCharArrayLength(targetCharArray);
    if (mainLength >= targetLength)
    {
        int index = 0;
        bool isChecking = false;
        bool hasIndex = false;
        int stringIndex = -1;
        int targetIndex = 0;
        while ((char)mainCharArray[index] != '\0')
        {
            if (isChecking)
            {
                if (targetIndex < targetLength - 1)
                {
                    if (mainCharArray[index] != targetCharArray[targetIndex])
                    {
                        isChecking = false;
                    }
                }
                else
                {
                    hasIndex = true;
                    stringIndex = index - (targetLength - 1);
                    break;
                }
            }
            if (!isChecking)
            {
                if (mainCharArray[index] == targetCharArray[0])
                {
                    isChecking = true;
                    targetIndex = 0;
                }
            }
            index++;
            if (isChecking)
            {
                targetIndex++;
            }
        }
        return stringIndex;
    }
    std::cout << "INVALID TARGET STRING" << std::endl;
    return -1;
}

// Get ending Index of a char array in another char array
int GetEndIndexString(char *mainCharArray, const char *targetCharArray)
{
    int mainLength = GetCharArrayLength(mainCharArray);
    int targetLength = GetCharArrayLength(targetCharArray);
    if (mainLength >= targetLength)
    {
        int index = GetStartIndexString(mainCharArray, targetCharArray);
        index += targetLength - 1;
        return index;
    }
    std::cout << "INVALID TARGET STRING" << std::endl;
    return -1;
}

// Get ending Index of a char array in another char array
int GetEndIndexString(const char *mainCharArray, const char *targetCharArray)
{
    int mainLength = GetCharArrayLength(mainCharArray);
    int targetLength = GetCharArrayLength(targetCharArray);
    if (mainLength >= targetLength)
    {
        int index = GetStartIndexString(mainCharArray, targetCharArray);
        index += targetLength - 1;
        return index;
    }
    std::cout << "INVALID TARGET STRING" << std::endl;
    return -1;
}

#endif