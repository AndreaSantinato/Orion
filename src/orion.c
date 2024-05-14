#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include "libs/core.h"
#include "libs/fileio.h"

/// @brief Defines all the available operations For
typedef enum OperationTypes
{
    CopyFile = 0,
    MoveFile = 1,
    RemoveFile = 2,
    CopyDirectory = 3,
    MoveDirectory = 4,
    RemoveDirectory = 5
} OperationTypes;

/// @brief Structure Type For A New Operation
typedef struct Operation
{
    OperationTypes type;
    char *source;
    char *destination;
} Operation;

/// @brief Define a new operation structure
/// @param type Contains the type of the operation
/// @param source Contains the source of the path/file
/// @param destination Contains the destination of the path/file
/// @return A new operation
Operation NewOperation(OperationTypes type, char *source, char *destination)
{
    Operation op = {type, source, destination};
    return op;
}

void ValidateOperationArguments(Operation operation)
{
    if (operation.source == NULL && operation.destination == NULL)
    {
        perror("No valid source and destination provided!");
    }
    else if (operation.source == NULL && operation.destination == NULL) 
    {
        perror("No valid source provided!");
    }
    else if (operation.source == NULL && operation.destination == NULL)
    {
        perror("No valid destination provided!");
    }

    if (operation.type == 0 || operation.type == 1 || operation.type == 2)
    {
        printf("Source: %s \n", operation.source);
        printf("Destination: %s \n", operation.destination);

        size_t sourceLen = strlen(operation.source);
        size_t destLen = strlen(operation.destination);

        // Check if destination ends with '/' or is an empty string
        if (destLen > 0 && (operation.destination[destLen - 1] == '/' || operation.destination[destLen - 1] == '\0'))
        {
            const char *lastSlash = strrchr(operation.source, '/');

            if (lastSlash != NULL)
            {
                // Calculate the index of the last '/'
                int lastIndex = lastSlash - operation.source;

                // Extract the substring after the last '/'
                char *lastPart;
                SubString(operation.source, lastIndex + 1, sourceLen - lastIndex - 1, &lastPart);

                // Append the extracted substring to the destination
                char *newDestination;
                SubString(operation.destination, 0, destLen - 1, &newDestination); // Exclude the trailing '/'
                strcat(newDestination, "/");
                strcat(newDestination, lastPart);
                strcpy(operation.destination, newDestination);

                free(lastPart);
            }
            else
            {
                strcat(operation.destination, operation.source);
            }
        }  
    }
    else if (operation.type == 3 || operation.type == 4 || operation.type == 5)
    {
        //
        // ToDo: Add the validations for the directory management
        //
    }
}

/// @brief Perform the provided operation
/// @param operation Contains all the information for an operation
void ExecuteOperation(Operation operation) 
{
    ValidateOperationArguments(operation);

    switch (operation.type)
    {
        case 0:
            {
                // Copy
                FileCopy(operation.source, operation.destination);
                break;
            }
        case 1:
            {
                // Move
                FileMove(operation.source, operation.destination);
                break;
            }
        case 2:
            {
                // Remove
                FileRemove(operation.source);
                break;
            }
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
    }
}

// Main entry point of the program
int main(int argc, char *argv[]) 
{
    if (argc <= 1) 
    {
        fprintf(stderr, "No arguments provided!\n");
        return 1;
    }

    // Variables Declaration
    int operationCode = -1;
    Operation operation;

    // Parse command-line options
    while ((operationCode = getopt(argc, argv, "c:m:r:")) != -1)
    {
        char *sourceFile = NULL;
        char *destinationFile = NULL;

        switch (operationCode)
        {
            case 'c':
                {
                    sourceFile = optarg;
                    if (!(optind < argc))
                    {
                        fprintf(stderr, "Missing second file path argument\n");
                        return 1;
                    }
                    destinationFile = argv[optind];
                    operation = NewOperation(0, optarg, argv[optind]);
                    break;
                }
            case 'm':
                {
                    sourceFile = optarg;
                    if (!(optind < argc))
                    {
                        fprintf(stderr, "Missing second file path argument\n");
                        return 1;
                    }
                    destinationFile = argv[optind];
                    operation = NewOperation(1, optarg, argv[optind]);
                    break;
                }
            case 'r':
                {
                    operation = NewOperation(0, optarg, NULL);
                    break;
                }
            default:
                {
                    fprintf(stderr, "Usage: %s -c source_file destination_file | -m source_file destination_file | -r source_file \n", argv[0]);
                    return 1;
                }
        }

        // Determine which method to call based on the option
        ExecuteOperation(operation);
    }

    return 0;
}