// ---------------------------------------------------------
//      Usr:    Andrea Santinato
//      Dscr:   File I/O Functions
// ---------------------------------------------------------

#ifndef FILEIO_H
#define FILEIO_H

/// @brief Remove the source file fromt the system
/// @param source Contain the full path of the source file
/// @return The Status of the operation (0 = Success; 1 = Error)
int RemoveFile(const char *source, bool outputOperations);

/// @brief This function move a source file into a new destination path
/// @param source Contains the source file
/// @param destination Contains the destination File/Path
/// @return The Status of the operation (0 = Success; 1 = Error)
int CopyFile(const char *source, const char *destination, bool outputOperations);

/// @brief Create a backup version of the source file
/// @param source Contain the original source file
/// @return True/False Status
int BackupFile(const char *source, bool outputOperations);

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

