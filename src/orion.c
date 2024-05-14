#include "libs/core.h"
#include "libs/utils.h"

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

/// @brief Perform the provided operation
/// @param operation Contains all the information for an operation
void ExecuteOperation(Operation operation) 
{
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