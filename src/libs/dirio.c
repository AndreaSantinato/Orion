#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include "core.h"
#include "dirio.h"

DirectoryContent RetrieveDirectoryContent(const char *source)
{
    struct dirent *entry;
    DIR *dp = opendir(source);
    DirectoryContent dir = {0, NULL};
    int capacity = 10; // Initial capacity of the items array

    if (dp == NULL)
    {
        perror("opendir");
        return dir;
    }

    dir.items = malloc(capacity * sizeof(char *));
    if (dir.items == NULL)
    {
        perror("malloc");
        closedir(dp);
        return dir;
    }

    while ((entry = readdir(dp)))
    {
        // Skip the entries for the current and parent directories (".", "..")
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
        {
            continue;
        }

        if (dir.count >= capacity)
        {
            capacity *= 2;
            dir.items = realloc(dir.items, capacity * sizeof(char *));
            if (dir.items == NULL)
            {
                perror("realloc");
                closedir(dp);
                return dir;
            }
        }

        dir.items[dir.count] = strdup(entry->d_name);
        if (dir.items[dir.count] == NULL)
        {
            perror("strdup");
            closedir(dp);
            for (int i = 0; i < dir.count; i++)
            {
                free(dir.items[i]);
            }
            free(dir.items);
            dir.items = NULL;
            dir.count = 0;
            return dir;
        }

        dir.count++;
    }

    closedir(dp);
    return dir;
}

void FreeDirectoryContent(DirectoryContent *directory)
{
    for (int i = 0; i < directory->count; i++)
    {
        free(directory->items[i]);
    }

    free(directory->items);

    directory->items = NULL;
    directory->count = 0;
}