#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include "core.h"
#include "dirio.h"

DirectoryContent RetrieveDirectoryContent(const char *source)
{
    DirectoryContent dir = {0, NULL};

    struct dirent *entry;
    int capacity = 10;

    // Open the directory
    DIR *dp = opendir(source);
    if (NULL == dp)
    {
        PrintOutput(MSG_ERROR, "No directory founded by the provided source (%s)", source);

        return dir;
    }

    // Pre-Allocate an initial memory for the retrieved elements
    dir.items = malloc(capacity * sizeof(char *));
    if (NULL == dir.items)
    {
        PrintOutput(MSG_ERROR, "An error occured during the allocation of the memory for the directory elements");
        
        closedir(dp);

        return dir;
    }

    // Read the content of the directory and store them inside the created structure
    while ((entry = readdir(dp)))
    {
        // Skip the entries for the current and parent directories (".", "..")
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
            continue;

        // if the element's count is higher than the original capacity it will reallocate the memory  to much the required 
        // amount of memory space to store all the elements
        if (dir.count >= capacity)
        {
            capacity *= 2;
            dir.items = realloc(dir.items, capacity * sizeof(char *));
            if (NULL == dir.items)
            {
                PrintOutput(MSG_ERROR, "An error occured during the re-allocation of the memory for the directory elements");
                
                closedir(dp);
                
                return dir;
            }
        }

        // Proceeds to store the current element inside the final array
        dir.items[dir.count] = strdup(entry->d_name);
        if (NULL == dir.items[dir.count])
        {
            PrintOutput(MSG_ERROR, "An error occured during retrieve of the directory's content");

            closedir(dp);
            
            // Free the allocated memory for each element store inside the array
            FreeDirectoryContent(&dir);
            
            return dir;
        }

        dir.count++;
    }

    closedir(dp);

    // Return the structure that store all the retrieved elements and their total count
    return dir;
}

void FreeDirectoryContent(DirectoryContent *directory)
{
    // Deallocate all the elements stored in the array from the memory
    for (int i = 0; i < directory->count; i++)
    {
        free(directory->items[i]);
    }

    // Deallocate the array from the mmemory
    free(directory->items);

    // Reset the values of the structure
    directory->items = NULL;
    directory->count = 0;
}

bool IsSourceDirectory(const char *source)
{
    return (strEndWiths(source, '/'));
}