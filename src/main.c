#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <unistd.h>
#include "libs/core.h"
#include "libs/utils.h"

#define PACKAGE "orion"
#define LOCALEDIR "/usr/local/share/locale"
#define AUTHOR proper_name("Andrea Santinato")

/// @brief Contains all the args options of the application
static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"backup", optional_argument, 0, 'b'},
    {"interactive", no_argument, 0, 'i'},
    {"no-overwrite", no_argument, 0, 'n'},
    {"file-only", no_argument, 0, 'f'},
    {"move", no_argument, 0, 'm'},
    {"output", no_argument, 0, 'o'}
};

void help()
{
    printf("Usage Help Guide\n\n");

    printf("\t Copy File/Directory: %s [OPTION]... SOURCE DESTINATION\n", PACKAGE);
    printf("\t Move File/Directory: %s [OPTION]... [-m] SOURCE DESTINATION\n", PACKAGE);
    printf("\t Remove: %s [OPTION]... --remove[SOURCE]\n\n", PACKAGE);

    fputs("\
        -h, --help                          Display the help guide\n\
        -b, --backup                        Make a backup of the destination file (or files if destionation is a directory)\n\
        -b, --backup[SOURCE]                Make a backup of the destination's file\n\
        -i, --interactive                   Prompt a request to the user before overwriting existing destination file\n\
        -n, --no-overwrite                  Skip the overwriting of an existing file\n\
        -f, --file-only                     Copy/Move only files, skipping directories\n\
        -m, --move                          Move a source file into a destination directory\n\
        -r, --remove[SOURCE]                Remove a source file\n\
        -R, --remove-directory[SOURCE]      Remove a source directory and all the content\n\
        -o, --output                        Print an output for every single operation\
    ", stdout);

    exit(EXIT_SUCCESS);
}

int perform(char *source, char *destination, Options *options)
{
    if (!strEndWiths(source, '/') || strEndWiths(source, '*'))
    {
        if (options->MoveAll)
        {
            // Copy the source file in the destination directory
            if (EXIT_SUCCESS != MoveFile(source, destination, options))
                return EXIT_FAILURE;
        }
        else
        {
            // Copy the source file in the destination directory
            if (EXIT_SUCCESS != CopyFile(source, destination, options))
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

void init_op_options(Options *options)
{
    options->DisplayOperationOutput = false;
    options->MakeBackup = false;
    options->MoveAll = false;
    options->MoveOnlyFiles = false;
    options->NoOverwriteFiles = false;
    options->UserInteraction = false;
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
        help();

    init_op_options(&op_options);

    // Parse command-line options
    int opt = -1;
    while ((opt = getopt_long(argc, argv, "hbinfmo", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
            {
                help();
                break;
            }
            case 'b':
            {
                op_options.MakeBackup = true;

                //if (optarg)
                //    backupFile = optarg;

                break;
            }
            case 'i':
            {
                op_options.UserInteraction = true;
                break;
            }
            case 'n':
            {
                op_options.NoOverwriteFiles = true;
                break;
            }
            case 'f':
            {
                op_options.MoveOnlyFiles = true;
                break;
            }
            case 'm':
            {
                op_options.MoveAll = true;
                break;
            }
            case 'o':
            {
                op_options.DisplayOperationOutput = true;
                break;
            }
            default:
            {
                help();
                break;
            }
        }
    }

    if (argc - optind != 2)
        help();

    // Retrieve the passed source and destination
    //      - files[0] => Indicates the source file/directory
    //      - files[1] => Indicates the destination file/directory
    files[0] = argv[optind];
    files[1] = argv[optind + 1];

    int op_status = perform(files[0], files[1], &op_options);

    exit(op_status ? EXIT_SUCCESS : EXIT_FAILURE);
}