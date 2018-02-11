#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#define PATH "./sampleDB/"

int main() {

    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(PATH); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    
    printf("Number of files in '%s': %d\n", PATH, file_count);
}
