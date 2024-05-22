#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include "libs/core.h"
#include "libs/fileio.h"

/// @brief Contains all the args options of the application
static struct option long_options[] = {
    {"move", no_argument, 0, 'm'},
    {"remove", no_argument, 0, 'r'},
    {"directory", no_argument, 0, 'd'},
    {"output", no_argument, 0, 'o'},
    {"backup", no_argument, 0, 'b'},
    {0, 0, 0, 0}
};

int do_file_operation(char *sourceFile, char *destinationFile, operation_options *options)
{
    if (options->BackupSource)
        FileBackup(sourceFile, options->OutputPrompt);

    if (!FileCopy(sourceFile, destinationFile, options->MoveSource, options->OutputPrompt))
    {
        perror("An error occured during the source file copy!");
        return EXIT_FAILURE;
    }

    if (options->RemoveSource || options->MoveSource)
    {
        if (!FileRemove(sourceFile, options->OutputPrompt))
        {
            perror("An error occured during the source file remove!");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int do_directory_operation(char *sourceDirectory, char *destinationDirectory, operation_options *options)
{
    //
    // ToDo: implement the logics here
    //

    return EXIT_SUCCESS;
}

/// @brief Main of the program
/// @param argc Contains the total number of passed arguments
/// @param argv Contains the values of passed arguments
/// @return
int main(int argc, char *argv[])
{
    operation_options op_options;
    char *files[2];

    if (argc <= 1) 
    {
        fprintf(stderr, "No arguments provided!\n");
        return 1;
    }

    op_options = op_options_init();

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
        fprintf(stderr, "Usage: %s [-d] [-r] source destination\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Indicates the source file/directory
    files[0] = argv[optind];

    // Indicates the destination file/directory
    files[1] = argv[optind + 1];

    if (op_options.OutputPrompt)
    {
        printf("[Info] Backup Source -> %d\n", op_options.BackupSource);
        printf("[Info] Is Directory -> %d\n", op_options.IsDirectory);
        printf("[Info] Move Source -> %d\n", op_options.MoveSource);
        printf("[Info] Remove Source -> %d\n\n", op_options.RemoveSource);
    }

    int op_status;
    if (!op_options.IsDirectory && !strEndWiths(files[0], '/'))
    {
        // File Operations
        op_status = do_file_operation(files[0], files[1], &op_options);
    }
    else
    {
        // Directory Operations
        op_status = do_directory_operation(files[0], files[1], &op_options);
    }

    return EXIT_SUCCESS;
}
