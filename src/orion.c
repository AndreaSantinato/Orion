// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
// Custom Libraries
#include "fileio.h"

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

// Main entry point of the program
int main(int argc, char *argv[]) {
    if (argc <= 1) 
    {
        fprintf(stderr, "No arguments provided!\n");
        return 1;
    }

    // Variables Declaration
    int operation = -1;
    char *sourceFile = NULL;
    char *destinationFile = NULL;

    // Parse command-line options

    while ((operation = getopt(argc, argv, "c:m:r:")) != -1)
    {
        switch (operation)
        {
            case 'c':
                sourceFile = optarg;
                if (!(optind < argc))
                {
                    fprintf(stderr, "Missing second file path argument\n");
                    return 1;
                }
                destinationFile = argv[optind];
                break;
            case 'm':
                sourceFile = optarg;
                if (!(optind < argc))
                {
                    fprintf(stderr, "Missing second file path argument\n");
                    return 1;
                }
                destinationFile = argv[optind];
                break;
            case 'r':
                sourceFile = optarg;     
                break;
            default:
                fprintf(stderr, "Usage: %s -c source_file destination_file | -m source_file destination_file | -r source_file \n", argv[0]);
                return 1;
        }

        // Determine which method to call based on the option
        if (operation == 'c')
        {
            FileCopy(sourceFile, destinationFile);   
        }
        else if (operation == 'm')
        {
            FileMove(sourceFile, destinationFile);
        }
        else if (operation == 'r')
        {
            FileRemove(sourceFile);
        }
    }

    return 0;
}