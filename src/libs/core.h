// --------------------------------------------------------- 
//      Usr:    Andrea Santinato                             
//      Dscr:   Core Functions                                 
// ---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>

/// @brief Defines a set of options to provide into an operation
typedef struct _OperationOptins
{
    bool OutputPrompt;
    bool BackupSource;
    bool IsDirectory;
    bool MoveSource;
    bool RemoveSource;
} operation_options;

/// @brief Initialize a new operation's options
/// @return Default Set of Operation's Options
operation_options op_options_init()
{
    operation_options options;

    options.OutputPrompt = false;
    options.BackupSource = false;
    options.IsDirectory = false;
    options.MoveSource = false;
    options.RemoveSource = false;

    return options;
}

/// @brief Export a part of the string between a starting point and a specific lenght
/// @param str Contains the input string to export the required character
/// @param startIndex Indicates the starting point of the first character to export
/// @param length Indicates the lenght of the string to export
/// @return Return the export value
char *strSubString(const char *str, const int startIndex, const int length)
{
    size_t inputLength = strlen(str);

    // Allocate memory for the output string
    char *output = (char *)malloc((length + 1) * sizeof(char)); // +1 for the null terminator
    if (output == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy the substring from the input to the output
    strncpy(output, str + startIndex, length);
    (output)[length] = '\0';

    return output;
}

/// @brief Check if a string start with a specific character
/// @param str Contains the string to check
/// @param character Contains the starting character to check
/// @return True/False Result
bool strStartWiths(const char *str, const char character)
{
    size_t len = strlen(str);
    return (len > 0 && str[0] == character);
}

/// @brief Check if a string end with a specific character
/// @param str Contains the string to check
/// @param character Contains the ending character to check
/// @return True/False Result
bool strEndWiths(const char *str, char character)
{
    size_t len = strlen(str);
    return (len > 0 && str[len - 1] == character);
}

/// @brief Check if a string is null or empty
/// @param str Contains the string to check
/// @return True/False Status
bool strNullOrEmpty(const char* str)
{
    size_t size = strlen(str);
    return (str == NULL || size == 0 || str[0] == '\0');
}