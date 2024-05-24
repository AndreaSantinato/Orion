// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   Directory I/O Functions
// ---------------------------------------------------------

#ifndef DIRIO_H
#define DIRIO_H

/// @brief Structure that store all the content of a directory and their total count
typedef struct DirectoryContent
{
    int count;
    char **items;
} DirectoryContent;

/// @brief Retrieve all the content of a source directory
/// @param source Contains the source directory that store all the files to retrieve
/// @param items Contains the total number of all the files retrieved
/// @return A directory structure that store all the required informations
DirectoryContent RetrieveDirectoryContent(const char *source);

/// @brief Free all the allocated memory for a directory strucutre
/// @param directory Contains the structure that store all the directory informations
void FreeDirectoryContent(DirectoryContent *directory);

#endif // DIRIO_H