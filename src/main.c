#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "libs/core.h"
#include "libs/fileio.h"
#include "libs/dirio.h"

    /// @brief Contains all the args options of the application
    static struct option long_options[] = {
        {"move", no_argument, 0, 'm'},
        {"remove", no_argument, 0, 'r'},
        {"directory", no_argument, 0, 'd'},
        {"output", no_argument, 0, 'o'},
        {"backup", no_argument, 0, 'b'},
        {0, 0, 0, 0}};

int perform(char *source, char *destination, Options *options)
{
    if (!strEndWiths(source, '/') || strEndWiths(source, '*'))
    {
        if (options->BackupSource)
        {
            // Backup the source file inside the source directory
            if (EXIT_SUCCESS != BackupFile(source, options->OutputPrompt))
                return EXIT_FAILURE;
        }

        if (options->MoveSource)
        {
            // Copy the source file in the destination directory
            if (EXIT_SUCCESS != CopyFile(source, destination, options->OutputPrompt))
                return EXIT_FAILURE;

            // Remove the source file
            if (EXIT_SUCCESS != RemoveFile(source, options->OutputPrompt))
                return EXIT_FAILURE;
        }
        else
        {
            // Copy the source file in the destination directory
            if (EXIT_SUCCESS != CopyFile(source, destination, options->OutputPrompt))
                return EXIT_FAILURE;
        }
    }
    else
    {
        // Remove the * character from the source if contains it
        if(strEndWiths(source, '*'))
            source = strSubString(source, 0, strlen(source) - 2);

        // Retrieve all the content of the specified directory
        DirectoryContent dir = RetrieveDirectoryContent(source);

        // Perform the required operation for each element
        for (int i = 0; i < dir.count; i++)
        {
            char *originalSrc = malloc(strlen(source));
            strcpy(originalSrc, source);

            char *newSrc = malloc(strlen(originalSrc) + strlen(dir.items[i]));
            strcpy(newSrc, originalSrc);
            strcat(newSrc, dir.items[i]);

            printf("Current File: %s \n", newSrc);

            if (EXIT_SUCCESS != perform(newSrc, destination, options))
            {
                free(originalSrc);
                free(newSrc);
                return EXIT_FAILURE;
            }

            free(originalSrc);
            free(newSrc);
        }

        // Free all the retrieved directory content
        FreeDirectoryContent(&dir);
    }

    return EXIT_SUCCESS;
}

/// @brief Main of the program
/// @param argc Contains the total number of passed arguments
/// @param argv Contains the values of passed arguments
/// @return
int main(int argc, char *argv[])
{
    Options op_options;
    char *files[2];

    if (argc <= 1)
    {
        PrintOutput(MSG_ERROR, "No arguments provided!\n");
        return EXIT_FAILURE;
    }

    op_options = InitializeOperationOptions();

    // Parse command-line options
    int opt = -1;
    while ((opt = getopt_long(argc, argv, "mrdob", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'm':
        {
            op_options.MoveSource = true;
            break;
        }
        case 'r':
        {
            op_options.RemoveSource = true;
            break;
        }
        case 'd':
        {
            op_options.IsDirectory = true;
            break;
        }
        case 'o':
        {
            op_options.OutputPrompt = true;
            break;
        }
        case 'b':
        {
            op_options.BackupSource = true;
            break;
        }
        default:
        {
            fprintf(stderr, "Usage: %s [--copy|--move] [--remove] [--directory] source destination\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        }
    }

    if (argc - optind != 2)
    {
        PrintOutput(MSG_WARNING, "Usage: %s [-d] [-r] source destination\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Indicates the source file/directory
    files[0] = argv[optind];

    // Indicates the destination file/directory
    files[1] = argv[optind + 1];

    if (op_options.OutputPrompt)
    {
        PrintOutput(MSG_INFO, "Usage: %s [-d] [-r] source destination\n", argv[0]);
        PrintOutput(MSG_INFO, "Backup Source -> %d\n", op_options.BackupSource);
        PrintOutput(MSG_INFO, "Is A Directory -> %d\n", op_options.IsDirectory);
        PrintOutput(MSG_INFO, "Move Source -> %d\n", op_options.MoveSource);
        PrintOutput(MSG_INFO, "Remove Source -> %d\n\n", op_options.RemoveSource);
    }

    int op_status = perform(files[0], files[1], &op_options);
    
    if (EXIT_FAILURE == op_status)
    {
        //
        // ToDo: Decides what to output
        //
    }

    return op_status;
}