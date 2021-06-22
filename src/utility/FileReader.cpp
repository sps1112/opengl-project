#include <utility/FileReader.h>

int get_char_array_length(char *charArray)
{
    int index = 0;
    while ((char)charArray[index] != '\0')
    {
        index++;
    }
    return (index + 1);
}

int get_char_array_length(const char *charArray)
{
    int index = 0;
    while ((char)charArray[index] != '\0')
    {
        index++;
    }
    return (index + 1);
}

char *convert_to_char_array(const char *charArray)
{
    int length = get_char_array_length(charArray);
    char *newArray = new char[length];
    for (int i = 0; i < length; i++)
    {
        *(newArray + i) = *(charArray + i);
    }
    return newArray;
}

std::string convert_to_string(char *charArray)
{
    std::string s = "";
    for (int i = 0; i < get_char_array_length(charArray); i++)
    {
        s = s + charArray[i];
    }
    return s;
}

float convert_to_float(char *charArray)
{
    std::string s = convert_to_string(charArray);
    std::stringstream ss(s);
    float value{0};
    ss >> value;
    return value;
}

bool compare_with_char_array(char *a, char *b)
{
    bool isEqual = true;
    int l1 = get_char_array_length(a);
    int l2 = get_char_array_length(b);
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

char *get_line(char *charList, int startIndex)
{
    char *newLine = new char[get_char_array_length(charList)];
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

char *get_line_part(char *line, int startIndex, int endIndex)
{
    int length = endIndex - startIndex;
    char *newLine = new char[length + 1];
    if (length >= 0 && length <= get_char_array_length(line) && startIndex >= 0 && endIndex < get_char_array_length(line))
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

int get_char_index(char *charList, char targetChar)
{
    int targetIndex = -1;
    int length = get_char_array_length(charList);
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

int get_other_char_index(char *charList, int startIndex, char targetChar)
{
    int otherIndex = -1;
    int length = get_char_array_length(charList);
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

int get_line_end_index(char *charList, int startIndex)
{
    int index = startIndex;
    while ((char)charList[index] != '\n')
    {
        index++;
    }
    return index;
}

int get_line_start_index(char *charList, int startIndex)
{
    return (get_line_end_index(charList, startIndex) + 1);
}

int get_start_index_string(char *mainCharArray, char *targetCharArray)
{
    int mainLength = get_char_array_length(mainCharArray);
    int targetLength = get_char_array_length(targetCharArray);
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
    log_message("INVALID TARGET STRING");
    return -1;
}

int get_end_index_string(char *mainCharArray, char *targetCharArray)
{
    int mainLength = get_char_array_length(mainCharArray);
    int targetLength = get_char_array_length(targetCharArray);
    if (mainLength >= targetLength)
    {
        int index = get_start_index_string(mainCharArray, targetCharArray);
        index += targetLength - 1;
        return index;
    }
    log_message("INVALID TARGET STRING");
    return -1;
}

int skip_lines(char *charArray, int startIndex, int n)
{
    for (int i = 0; i < n; i++)
    {
        startIndex = get_line_start_index(charArray, startIndex);
    }
    return startIndex;
}
