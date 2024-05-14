// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>

/// @brief Defines all the available operations For
typedef enum
{
    Copy = 0,
    Move = 1,
    Remove = 2
} OperationTypes;

/// @brief Structure Type For A New Operation
typedef struct
{
    OperationTypes type;
    char *source;
    char *destination;
} Operation;

/// @brief Define a new operation structure
/// @param type Contains the type of the operation
/// @param source Contains the source of the path/file
/// @param destination Contains the destination of the path/file
/// @return 
Operation NewOperation(OperationTypes type, char *source, char *destination)
{
    Operation op = {type, source, destination};

    return op;
}

