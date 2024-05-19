// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   File I/O Functions
// ---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>

/// @brief Remove the source file fromt the system
/// @param source Contain the full path of the source file
/// @return The Status of the operation (0 = Success; 1 = Error)
bool FileRemove(const char *source, bool output)
{
    if (0 != remove(source))
    {
        printf("An error occured during the removing of the %s file!\n", source);
        return false;
    }

    if (output)
        printf("[Info] File %s Removed\n", source);

    return true;
}

/// @brief This function move a source file into a new destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
bool FileCopy(const char *source, const char *destination, bool isFileMove, bool output)
{
    FILE *sourceFile, *destinationFile;

    // Open the source file
    sourceFile = fopen(source, "r");
    if (sourceFile == NULL)
    {
        perror("No source file provided or found!");
        return false;
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
            perror("Memory allocation failed");
            fclose(sourceFile);
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
            perror("Memory allocation failed");
            fclose(sourceFile);
            return false;
        }
    }

    // Open the destination file
    destinationFile = fopen(newDst, "w");
    if (destinationFile == NULL)
    {
        perror("No destination file provided or found!");
        fclose(sourceFile);
        free(newDst);
        return false;
    }

    // Writing the content of the source file inside the destination file
    char character;
    while ((character = fgetc(sourceFile)) != EOF)
    {
        fputc(character, destinationFile);
    }

    // Closing the source and destination files
    fclose(sourceFile);
    fclose(destinationFile);

    if (output)
    {
        if (isFileMove)
            printf("[Info] Move: %s -> %s\n", source, newDst);
        else
            printf("[Info] Copy: %s -> %s\n", source, newDst);
    }

    free(newDst);

    return true;
}

/// @brief Create a backup version of the source file
/// @param source Contain the original source file
/// @return True/False Status
bool FileBackup(const char *source, bool output)
{
    const char *bakExtension = ".bak";

    char *bakFile = malloc(strlen(source) + strlen(bakExtension));
    strcat(bakFile, source);
    strcat(bakFile, bakExtension);

    bool ok = FileCopy(source, bakFile, false, false);

    if (!ok)
    {
        printf("An error occured during the backup of the %s file!\n", source);
        return false;
    }

    if (output)
        printf("[Info] Backup: %s -> %s\n", source, bakFile);
        
    return true;
}

/// @brief Create a new empty file inside a destination full path
/// @param filePath Contains the path where the new file will be created
/// @param fileName Contains the name of the new file to create
/// @param fileName Contains the new created destination path
/// @return 0 = Success; 1 = Error
char *newDestinationPath(const char *filePath, const char *fileName)
{
    char *newPath;

    // Calculate the length of the new path
    size_t filePathLen = strlen(filePath);
    size_t fileNameLen = strlen(fileName);
    size_t newPathLen = filePathLen + fileNameLen + 2; // +2 for the '/' and the null terminator

    // Allocate memory for the new path
    newPath[0] = '\0'; // Ensure the newPath is an empty string
    strcat(newPath, filePath);

    // If filePath doesn't end with '/', add it
    if (filePath[filePathLen - 1] != '/')
        strcat(newPath, "/");

    // Append the file name to the new path
    strcat(newPath, fileName);

    return newPath;
}

/// @brief Create a new empty file inside a destination full path
/// @param source Contains the full path where the new file will be created
/// @return 0 = Success; 1 = Error
int CreateEmptyFile(const char *source)
{
    // Create the new empty file
    FILE *file = fopen(source, "wb");

    // Close the file
    if (fclose(file) != 0)
    {
        perror("An error occurred while closing the file!");
        return 1;
    }

    return 0;
}