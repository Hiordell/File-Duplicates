#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdint.h>
#include"FileInfo.h"
#include"CheckOptions.h"
#include"GetFilesInfo.h"
#include"GetDuplicateList.h"

int main(int argc, char **argv)
{
    struct FileInfo *fileInfo = NULL;
    char *dirName = NULL;
    char *fileName = NULL;
    char *saveFilePath = NULL;
    int options[7] = {};
    int fileCount = 0;
    
    CheckOptions(argc, argv, &dirName, options, &fileName, &saveFilePath);

    if (options[5])
    {
        printf("\nUsage:");
        printf("\ngetdupes [options] DIRECTORY");
        printf("\nDescription:");
        printf("\nSearches  the  given  path for duplicate files. Such files are found by");
        printf("\ncomparing file sizes and MD5 signatures, followed by a byte-by-byte");
        printf("\ncomparison.");
        printf("\nOptions:");
        printf("\n-r                  for every directory given follow subdirectories encountered within");
        printf("\n-f FILENAME         save the result to a file");
        printf("\n-n                  exclude zero-length files from consideration");
        printf("\n-s                  show size of duplicate files");
        printf("\n-d                  prompt user for files to preserve and delete all others");
        printf("\n-h                  display this help message");
        printf("\n-N DUPLICATESNAME   filtering by filename\n");
        return 0;
    }

    DIR *dir = opendir(dirName);

	if (dir == NULL)
	{
		perror("Directory open error");
		exit(errno);
	}
    
    GetFilesInfo(dirName, dir, &fileInfo, &fileCount, options, fileName);

    closedir(dir);
    
    GetDuplicateList(fileInfo, fileCount, options, saveFilePath, fileName);

    return 0;
}