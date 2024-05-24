// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   File I/O Functions
// ---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include "core.h"
#include "core.h"
#include "fileio.h"

/// @brief Remove the source file fromt the system
/// @param source Contain the full path of the source file
/// @return The Status of the operation (0 = Success; 1 = Error)
int RemoveFile(const char *source, bool outputOperations)
{
    int op_result = remove(source);

    if (EXIT_SUCCESS != op_result)
    {
        PrintOutput(MSG_ERROR, "An error occured during the removing of the %s file!\n", source);
        return op_result;
    }

    if (outputOperations)
        PrintOutput(MSG_INFO, "File %s Removed\n", MSG_INFO, source);

    return EXIT_SUCCESS;
}

/// @brief This function move a source file into a new destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
int CopyFile(const char *source, const char *destination, bool outputOperations)
{
    FILE *srcFile, *dstFile;

    // Open the source file
    srcFile = fopen(source, "r");
    if (NULL == srcFile)
    {
        PrintOutput(MSG_ERROR, "No source file provided or found!");
        return EXIT_FAILURE;
    }

    char *newDst = NULL;
    if (strEndWiths(destination, '/'))
    {
        // Takes the name from the source and use it to create a new file inside the destination path
        const char *lastSlash = strrchr(source, '/');
        const char *name = (lastSlash != NULL) ? lastSlash + 1 : source;

        newDst = malloc(strlen(destination) + strlen(name) + 1);
        if (newDst == NULL)
        {
            PrintOutput(MSG_ERROR, "Memory allocation failed!");
            fclose(srcFile);
            return false;
        }

        strcpy(newDst, destination);
        strcat(newDst, name);
    }
    else
    {
        newDst = strdup(destination);
        if (newDst == NULL)
        {
            PrintOutput(MSG_ERROR, "Memory allocation failed!");
            fclose(srcFile);
            return false;
        }
    }

    // Open the destination file
    dstFile = fopen(newDst, "w");
    if (dstFile == NULL)
    {
        PrintOutput(MSG_ERROR, "No destination file provided or found!");
        fclose(srcFile);
        free(newDst);
        return false;
    }

    // Writing the content of the source file inside the destination file
    char character;
    while ( EOF != (character = fgetc(srcFile)) )
    {
        fputc(character, dstFile);
    }

    // Closing the source and destination files
    fclose(srcFile);
    fclose(dstFile);

    if (outputOperations)
        PrintOutput(MSG_INFO, "%s -> %s\n", source, newDst);

    free(newDst);

    return EXIT_SUCCESS;
}

/// @brief Create a backup version of the source file
/// @param source Contain the original source file
/// @return True/False Status
int BackupFile(const char *source, bool outputOperations)
{
    const char *bakExtension = ".bak";

    char *bakFile = malloc(strlen(source) + strlen(bakExtension));
    strcat(bakFile, source);
    strcat(bakFile, bakExtension);

    if (EXIT_SUCCESS != CopyFile(source, bakFile, false))
    {
        PrintOutput(MSG_ERROR, "An error occured during the backup of the %s file!\n", source);
        return EXIT_FAILURE;
    }

    if (outputOperations)
        PrintOutput(MSG_INFO, "Backup: %s -> %s\n", source, bakFile);

    return EXIT_SUCCESS;
}

/// @brief Create a new empty file inside a destination full path
/// @param source Contains the full path where the new file will be created
/// @return 0 = Success; 1 = Error
int NewEmptyFile(const char *source)
{
    // Create the new empty file
    FILE *file = fopen(source, "wb");

    // Close the file
    if (EXIT_SUCCESS != fclose(file))
    {
        PrintOutput(MSG_ERROR, "An error occurred while closing the new file!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/// @brief Create a new empty file inside a destination full path
/// @param filePath Contains the path where the new file will be created
/// @param fileName Contains the name of the new file to create
/// @param fileName Contains the new created destination path
/// @return 0 = Success; 1 = Error
char *NewDestinationPath(const char *filePath, const char *fileName)
{
    char *newPath;
    size_t filePathLen, fileNameLen, newPathLen;

    // Calculate the length of the new path
    filePathLen = strlen(filePath);
    fileNameLen = strlen(fileName);
    newPathLen = filePathLen + fileNameLen + 2; // +2 for the '/' and the null terminator

    // Allocate memory for the new path
    newPath[0] = '\0'; // Ensure the newPath is an empty string
    strcat(newPath, filePath);

    // If filePath doesn't end with '/', add it
    if ('/' != filePath[filePathLen - 1])
        strcat(newPath, "/");

    // Append the file name to the new path
    strcat(newPath, fileName);

    return newPath;
}