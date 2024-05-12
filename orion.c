#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>

/// @brief Update a destination using the file's name included in the source
/// @param source Contains the source that contains the file's name
/// @param fileName Indicates the extracted file's name from the source
void ExtractFileNameFromSource(const char *source, char **fileName)
{
    // Find the last occurrence of '/' in the source path
    const char *last_slash = strrchr(source, '/');

    if (last_slash == NULL)
    {
        // If no slash is found, copy the entire source path
        *fileName = malloc(strlen(source) + 1);
        if (*fileName == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strcpy(*fileName, source);
    }
    else
    {
        // Copy the filename after the last slash
        *fileName = malloc(strlen(last_slash + 1) + 1);
        if (*fileName == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strcpy(*fileName, last_slash + 1);
    }
}

/// @brief Create a new empty file inside a destination full path
/// @param filePath Contains the path where the new file will be created
/// @param fileName Contains the name of the new file to create
/// @return 0 = Success; 1 = Error
int CreateEmptyFile(const char *filePath, const char *fileName) {
    char *fileFullPath = malloc(sizeof(filePath) + sizeof(fileName));

    strcat(fileFullPath, filePath);
    strcat(fileFullPath, "/");
    strcat(fileFullPath, fileName);

    printf("[Debug] New File Path: %s\n", fileFullPath);

    FILE *file = fopen(fileFullPath, "wb");

    if (0 != fclose(file)) {
        perror("An error occured during the creation of the new empty file!");
        free(fileFullPath);
        return 1;
    }

    free(fileFullPath);
    
    return 0;
}

/// @brief This function move a source file into a new destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
int MoveFile(const char *source, const char *destination, const int removeSource) {
    FILE *source_file;
    FILE *destination_file;

    printf("[Debug] Source: %s \n", source);
    printf("[Debug] Destination: %s \n", destination);

    //
    // ToDo: Move the file into the new path
    //

    // Open the source file for reading
    source_file = fopen(source, "rb");
    if (source_file == NULL)
    {
        perror("There was an error opening the source file!");
        return 1;
    }

    // Open the destination file for writing, if not exist it will be created
    destination_file = fopen(destination, "wb");
    if (destination_file == NULL)
    {
        // The destination file doesn't exist so it will create a new one
        // using the same name of the source file

        char *extractedFileName;
        ExtractFileNameFromSource(source, &extractedFileName);

        if (0 != CreateEmptyFile(destination, extractedFileName))
        {
            perror("An error occured while creating the destination file!");

            fclose(source_file);
            free(extractedFileName);
            
            return 1;
        }

        free(extractedFileName);
    }

    if (removeSource == 1) {
        //
        // ToDo: Remove the source
        //
    }

    return 0;
}

// Main entry point of the program
int main(int argc, char *argv[]) {
    if (argc <= 1) 
    {
        printf("No arguments provided!\n");
        return 1;
    }

    int result = MoveFile(argv[1], argv[2], 0);

    return 0;
}