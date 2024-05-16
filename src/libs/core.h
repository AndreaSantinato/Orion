#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>

/// @brief Defines a more readable string type
typedef struct String
{
    size_t Size;
    char *Value;
} String;

/// @brief Create a new string
/// @param size Define how many space will be allocated into the memory
/// @param value Contains the actual value of the string
/// @return A new string
String NewString(size_t size, char *value)
{
    String str;

    str.Size=size;
    str.Value = malloc(size + 1);

    if (str.Value != NULL)
    {
        strcpy(str.Value, value);
        str.Value[size + 1] = '\0';
    }

    return str;
}

/// @brief Deallocate the string from the memory
/// @param str Contains the pointer of the string inside the memory
void FreeString(String *str)
{
    if (str->Value != NULL)
    {
        free(str->Value);
        str->Value = NULL;
    }
}

/// @brief Export a part of the string between a starting point and a specific lenght
/// @param input Contains the input string to export the required character
/// @param startIndex Indicates the starting point of the first character to export
/// @param length Indicates the lenght of the string to export
/// @param output Contains the export string value
void SubString(const char *input, const int startIndex, const int length, char **output)
{
    size_t inputLength = strlen(input);
    // Allocate memory for the output string
    *output = (char *)malloc((length + 1) * sizeof(char)); // +1 for the null terminator
    if (*output == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }
    // Copy the substring from the input to the output
    strncpy(*output, input + startIndex, length);
    (*output)[length] = '\0'; // Null-terminate the output string
}

int StartWiths(const char *source, const char character)
{
    return 0;
}

int EndWiths(const char *source, const char character)
{
    return 0;
}