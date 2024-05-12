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
int FileRemove(const char *source)
{
    if (0 != remove(source))
    {
        fprintf(stderr, "Impossible to remove the source file!");
        return 1;
    }

    return 0;
}

/// @brief This function move a source file into a new destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
int FileCopy(const char *source, const char *destination)
{
    FILE *sourceFile = fopen(source, "r");
    FILE *destinationFile = fopen(destination, "w");

    if (sourceFile == NULL)
    {
        perror("No source file provided or founded!");
        return 1;
    }

    if (destinationFile == NULL)
    {
        perror("No destination file provided or founded!");
        return 1;
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

    printf("File moved successfully.\n");

    return 0;
}

/// @brief Copy the content of a source file inside a destination file and then remove the source one
/// @param source Contain the full path of the source file
/// @param destination Contains the full path of the destination
/// @return The Status of the operation (0 = Success; 1 = Error)
int FileMove(const char *source, const char *destination)
{
    // Copy the content of the source file into the destination file
    // After that it will remove the source file from the system
    FileCopy(source, destination);
    FileRemove(source);

    return 0;
}

/// @brief Create a new empty file inside a destination full path
/// @param filePath Contains the path where the new file will be created
/// @param fileName Contains the name of the new file to create
/// @return 0 = Success; 1 = Error
int CreateEmptyFile(const char *filePath, const char *fileName)
{
    char fileFullPath[sizeof(filePath) + sizeof(fileName)];

    strcat(fileFullPath, filePath);
    strcat(fileFullPath, "/");
    strcat(fileFullPath, fileName);

    FILE *file = fopen(fileFullPath, "wb");

    if (0 != fclose(file))
    {
        perror("An error occured during the creation of the new empty file!");
        return 1;
    }

    return 0;
}