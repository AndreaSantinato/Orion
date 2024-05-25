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
#include "fileio.h"

int RemoveFile(const char *source, Options *options)
{
    if (options->UserInteraction && !AskUserAuthorizationForOperation("Remove the source file [Y/N]?"))
        return EXIT_SUCCESS;

    int op_result = remove(source);

    if (EXIT_SUCCESS != op_result)
    {
        PrintOutput(MSG_ERROR, "An error occured during the removing of the %s file!\n", source);
        return op_result;
    }

    if (options->DisplayOperationOutput)
        PrintOutput(MSG_INFO, "File %s Removed\n", MSG_INFO, source);

    return EXIT_SUCCESS;
}

int CopyFile(const char *source, const char *destination, Options *options)
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

    // If required it make a backup of the destination file if it exist
    if (options->MakeBackup)
    {
        const char *bakExtension = ".bak";

        char *bakFile = malloc(strlen(destination) + strlen(bakExtension));
        strcat(bakFile, destination);
        strcat(bakFile, bakExtension);

        options->MakeBackup = false;

        if (EXIT_SUCCESS != CopyFile(destination, bakFile, options))
        {
            options->MakeBackup = true;
            PrintOutput(MSG_ERROR, "An error occured during the backup of the %s file!\n", destination);
            return EXIT_FAILURE;
        }

        options->MakeBackup = true;

        if (options->DisplayOperationOutput)
            PrintOutput(MSG_INFO, "Backup: %s -> %s\n", source, bakFile);
    }

    if (options->UserInteraction && !AskUserAuthorizationForOperation("Proceeds with the copy of the source file [Y/N]?"))
        return EXIT_SUCCESS;

    // Writing the content of the source file inside the destination file
    char character;
    while ( EOF != (character = fgetc(srcFile)) )
    {
        fputc(character, dstFile);
    }

    // Closing the source and destination files
    fclose(srcFile);
    fclose(dstFile);

    if (options->DisplayOperationOutput)
        PrintOutput(MSG_INFO, "%s -> %s\n", source, newDst);

    free(newDst);

    return EXIT_SUCCESS;
}

int MoveFile(const char *source, const char *destination, Options *options)
{
    // Copy the source fikle inside the the destination path/file
    if (EXIT_SUCCESS != CopyFile(source, destination, options))
        return EXIT_FAILURE;

    // Remove the source file
    if (EXIT_SUCCESS != RemoveFile(source, options))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

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