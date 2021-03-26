#include <utility/FileReader.h>

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
float ConvertToFloat(char *charArray)
{
    std::string s = ConvertToString(charArray);
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

// Returns part of line from start to end index(excluding the endIndex)
char *GetLinePart(char *line, int startIndex, int endIndex)
{
    int length = endIndex - startIndex;
    char *newLine = new char[length + 1];
    if (length >= 0 && length <= GetCharArrayLength(line) && startIndex >= 0 && endIndex < GetCharArrayLength(line))
    {
        for (int i = 0; i < length; i++)
        {
            char c = *(line + startIndex + i);
            *(newLine + i) = *(line + startIndex + i);
        }
        *(newLine + length) = '\0';
    }
    return newLine;
}

int GetCharIndex(char *charList, char targetChar)
{
    int targetIndex = -1;
    int length = GetCharArrayLength(charList);
    for (int i = 0; i < length; i++)
    {
        if (*(charList + i) == targetChar)
        {
            targetIndex = i;
            break;
        }
    }
    return targetIndex;
}

int GetOtherCharIndex(char *charList, int startIndex, char targetChar)
{
    int otherIndex = -1;
    int length = GetCharArrayLength(charList);
    if (startIndex > 0 && startIndex < length)
    {
        for (int i = startIndex; i < length; i++)
        {
            if (*(charList + i) != targetChar)
            {
                otherIndex = i;
                break;
            }
        }
    }
    return otherIndex;
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

int SkipLines(char *charArray, int startIndex, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        startIndex = GetLineStartIndex(charArray, startIndex);
    }
    return startIndex;
}