// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   Utility Functions
// ---------------------------------------------------------

#ifndef UTILS_H
#define UTILS_H

/// @brief Structure that store all the content of a directory and their total count
typedef struct DirectoryContent
{
    int count;
    char **items;
} DirectoryContent;

/// @brief Remove the source file fromt the system
/// @param source Contain the full path of the source file
/// @return The Status of the operation (0 = Success; 1 = Error)
int RemoveFile(const char *source, Options *options);

/// @brief This function copy a source file inside a destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
int CopyFile(const char *source, const char *destination, Options *options);

/// @brief This function move a source file inside a destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
int MoveFile(const char *source, const char *destination, Options *options);

/// @brief Create a new empty file inside a destination full path
/// @param source Contains the full path where the new file will be created
/// @return 0 = Success; 1 = Error
int NewEmptyFile(const char *source);

/// @brief Create a new empty file inside a destination full path
/// @param filePath Contains the path where the new file will be created
/// @param fileName Contains the name of the new file to create
/// @param fileName Contains the new created destination path
/// @return 0 = Success; 1 = Error
char *NewDestinationPath(const char *filePath, const char *fileName);

/// @brief Retrieve all the content of a source directory
/// @param source Contains the source directory that store all the files to retrieve
/// @param items Contains the total number of all the files retrieved
/// @return A directory structure that store all the required informations
DirectoryContent RetrieveDirectoryContent(const char *source);

/// @brief Free all the allocated memory for a directory strucutre
/// @param directory Contains the structure that store all the directory informations
void FreeDirectoryContent(DirectoryContent *directory);

/// @brief Check if the provided source is a directory or a file
/// @param source Contains the source to chek
/// @return A True/False Result
bool IsSourceDirectory(const char *source);

#endif // UTILS_H