// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   File I/O Functions
// ---------------------------------------------------------

#ifndef FILEIO_H
#define FILEIO_H

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

#endif // FILEIO_H