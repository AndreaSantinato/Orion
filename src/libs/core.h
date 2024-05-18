#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>

// ---------------------------------------------------- //
//                                                      //
//              String type Definitions                 //
//                                                      //
// ---------------------------------------------------- //

/// @brief Defines a more readable string type
typedef struct _String
{
    size_t Size;
    char *Value;
} string;

/// @brief Create a new string
/// @param size Define how many space will be allocated into the memory
/// @param value Contains the actual value of the string
/// @return A new string
string strnew(char *value)
{
    string str;
    size_t size = strlen(value);

    str.Size = size;
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
void strfree(string *str)
{
    if (str->Value != NULL)
    {
        free(str->Value);
        str->Value = NULL;
    }
}

// ------------------------------------------------------ //
//                                                        //
//              Operation Type Definitions                //
//                                                        //
// ------------------------------------------------------ //

/// @brief Defines all the available operations For
typedef enum _OperationTypes
{
    CopyFile = 0,
    MoveFile = 1,
    RemoveFile = 2,
    CopyDirectory = 3,
    MoveDirectory = 4,
    RemoveDirectory = 5
} operation_types;

/// @brief Structure Type For A New Operation
typedef struct _Operation
{
    operation_types type;
    char *source;
    char *destination;
} operation;

/// @brief Define a new operation structure
/// @param type Contains the type of the operation
/// @param source Contains the source of the path/file
/// @param destination Contains the destination of the path/file
/// @return A new operation
operation opnew(operation_types type, char *source, char *destination)
{
    operation op = {type, source, destination};
    return op;
}

/// @brief Remove all the values included inside the current operation from the memory
/// @param op Contains the operation free
void opfree(operation *op)
{
    if (op->source != NULL)
    {
        free(op->source);
        op->source=NULL;
    }

    if (op->destination != NULL)
    {
        free(op->destination);
        op->destination = NULL;
    }

    //strfree(&op->source);
    //strfree(&op->destination);
}

// --------------------------------------------------------- //
//                                                           //
//              Utility Functions Definitions                //
//                                                           //
// --------------------------------------------------------- //

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

/// @brief Check if a string start with a specific character
/// @param source Contains the string to check
/// @param character Contains the starting character to check
/// @return True/False Result
bool StartWiths(const char *source, const char character)
{
    bool result;
    char *value = malloc(1);

    SubString(source, 1, 1, &value);

    if (strchr(value, character))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    free(value);

    return result;
}

/// @brief Check if a string end with a specific character
/// @param source Contains the string to check
/// @param character Contains the ending character to check
/// @return True/False Result
bool EndWiths(const char *source, const char character)
{
    bool result;
    char *value = malloc(1);

    SubString(source, strlen(source) - 1, 1, &value);

    if (strchr(value, character))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    free(value);
    
    return result;
}