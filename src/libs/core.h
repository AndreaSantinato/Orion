// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   Core Functions
// ---------------------------------------------------------

#ifndef CORE_H
#define CORE_H

#include <stddef.h>
#include <stdbool.h>

#define MSG_INFO "INFO"
#define MSG_WARNING "WARN"
#define MSG_ERROR "ERR"

/// @brief Defines a set of options to provide into an operation
typedef struct Options
{
    bool OutputPrompt;
    bool BackupSource;
    bool IsDirectory;
    bool MoveSource;
    bool RemoveSource;
} Options;

/// @brief Initialize a new operation's options
/// @return Default Set of Operation's Options
Options InitializeOperationOptions();

/// @brief Print an output message to the console
/// @param type Define the type of the output (Info, Warn, Err)
/// @param str Contains the message to print
void PrintOutput(const char *type, const char *str, ...);

/// @brief Export a part of the string between a starting point and a specific length
/// @param str Contains the input string to export the required character
/// @param startIndex Indicates the starting point of the first character to export
/// @param length Indicates the length of the string to export
/// @return Return the export value
char *strSubString(const char *str, const int startIndex, const int length);

/// @brief Check if a string starts with a specific character
/// @param str Contains the string to check
/// @param character Contains the starting character to check
/// @return True/False Result
bool strStartWiths(const char *str, const char character);

/// @brief Check if a string ends with a specific character
/// @param str Contains the string to check
/// @param character Contains the ending character to check
/// @return True/False Result
bool strEndWiths(const char *str, char character);

/// @brief Check if a string is null or empty
/// @param str Contains the string to check
/// @return True/False Status
bool strNullOrEmpty(const char *str);

#endif // CORE_H
