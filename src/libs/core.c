// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   Core Functions
// ---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "core.h"

Options InitializeOperationOptions()
{
    Options options;

    options.OutputPrompt = false;
    options.BackupSource = false;
    options.IsDirectory = false;
    options.MoveSource = false;
    options.RemoveSource = false;

    return options;
}

void PrintOutput(const char *type, const char *str, ...)
{
    // Build the prefix of the output
    size_t prefix_len = strlen(type) + 3; // 3 for "[", "] ", and null terminator
    char *prefix = malloc(prefix_len);
    if (prefix == NULL)
    {
        perror("Memory allocation failed\n");
        return;
    }

    snprintf(prefix, prefix_len, "[%s] ", type);

    // Build the final output
    size_t msg_len = prefix_len + strlen(str) + 2; // 2 for ": " and null terminator
    char *msg = malloc(msg_len);
    if (msg == NULL)
    {
        perror("Memory allocation failed\n");
        free(prefix);
        return;
    }

    snprintf(msg, msg_len, "%s%s", prefix, str);

    va_list args;
    va_start(args, str);
    vprintf(msg, args);
    va_end(args);

    free(prefix);
    free(msg);
}

char *strSubString(const char *str, const int startIndex, const int length)
{
    size_t inputLength = strlen(str);

    // Allocate memory for the output string
    char *output = (char *)malloc((length + 1) * sizeof(char)); // +1 for the null terminator
    if (output == NULL)
    {
        PrintOutput(MSG_ERROR, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the substring from the input to the output
    strncpy(output, str + startIndex, length);
    output[length] = '\0';

    return output;
}

bool strStartWiths(const char *str, const char character)
{
    return (str != NULL && str[0] == character);
}

bool strEndWiths(const char *str, char character)
{
    size_t len = strlen(str);
    return (len > 0 && str[len - 1] == character);
}

bool strNullOrEmpty(const char *str)
{
    return (str == NULL || str[0] == '\0');
}
