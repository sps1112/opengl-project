#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <sstream>

// Calculates length of character array
int GetCharArrayLength(char *charArray)
{
    int index = 0;
    while ((char)charArray[index] != '\0')
    {
        index++;
    }
    return (index + 1);
}

int GetCharArrayLength(const char *charArray)
{
    int index = 0;
    while ((char)charArray[index] != '\0')
    {
        index++;
    }
    return (index + 1);
}

char *ConvertToCharArray(const char *charArray)
{
    int length = GetCharArrayLength(charArray);
    char *newArray = new char[length];
    for (int i = 0; i < length; i++)
    {
        *(newArray + i) = *(charArray + i);
    }
    return newArray;
}

// Converts character Array to string
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
    std::string s = ConvertToString(ConvertToCharArray(charArray));
    std::stringstream ss(s);
    float value{0};
    ss >> value;
    return value;
}

bool CompareCharArray(char *a, char *b)
{
    bool isEqual = true;
    int l1 = GetCharArrayLength(a);
    int l2 = GetCharArrayLength(b);
    if (l1 != l2)
    {
        isEqual = false;
    }
    else
    {
        for (int i = 0; i < l1; i++)
        {
            if (*(a + i) != *(b + i))
            {
                isEqual = false;
                break;
            }
        }
    }
    return isEqual;
}

// Gets Single Line from a starting Index till '\n'
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
int GetLineStartIndex(char *charList, int startIndex)
{
    return (GetLineEndIndex(charList, startIndex) + 1);
}

// Get starting Index of a char array in another char array
int GetStartIndexString(char *mainCharArray, char *targetCharArray)
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

// Get ending Index of a char array in another char array
int GetEndIndexString(char *mainCharArray, char *targetCharArray)
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