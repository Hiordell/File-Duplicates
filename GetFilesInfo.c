#include"GetFilesInfo.h"

void GetFilesInfo(char* dirName, DIR* dir, struct FileInfo** fileInfo, int* fileCount, int* options, char* fileName)
{
    struct dirent *dirEl;
    dirEl = readdir(dir);

    if (dirEl == NULL)
    {
        if (errno == EBADF)
        {
            perror("Invalid dir pointer");
            exit(errno);
        }

        return;
    }

    if (strcmp(dirEl->d_name, ".") && strcmp(dirEl->d_name, ".."))
	{
		if (dirEl->d_type == DT_DIR)
		{
			if (options[0])
			{
				char *newDirName;
                newDirName = (char*)calloc((strlen(dirName) + strlen(dirEl->d_name) + 2), sizeof(char));

                if (newDirName == NULL)
                {
                    perror("Memory allocation error");
                    exit(errno);
                }

				strncpy(newDirName, dirName, strlen(dirName));
				strcat(newDirName, dirEl->d_name);
                strcat(newDirName, "/");

				DIR *newDir = opendir(newDirName);

                if (newDir == NULL)
                {
                    perror("Directory open error");
                    exit(errno);
                }

				GetFilesInfo(newDirName, newDir, fileInfo, fileCount, options, fileName);
                closedir(newDir);
                free(newDirName);
			}
		}
		else
		{
			(*fileCount)++;

            if (*fileCount == 1)
            {
                *fileInfo = (struct FileInfo*)calloc(1, sizeof(struct FileInfo));

                if (*fileInfo == NULL)
                {
                    perror("Memory allocation error");
                    exit(errno);
                }
            }
            else
            {
                *fileInfo = (struct FileInfo*)realloc(*fileInfo, *fileCount * sizeof(struct FileInfo));

                if (*fileInfo == NULL)
                {
                    perror("Memory allocation error");
                    exit(errno);
                }
            }

			(*fileInfo + (*fileCount) - 1)->flagIgnore = 0;
            (*fileInfo + (*fileCount) - 1)->name = NULL;
            (*fileInfo + (*fileCount) - 1)->fileName = NULL;
            (*fileInfo + (*fileCount) - 1)->name = (char*)calloc((strlen(dirName) + strlen(dirEl->d_name) + 1), sizeof(char));
            (*fileInfo + (*fileCount) - 1)->fileName = (char*)calloc((strlen(dirEl->d_name) + 1), sizeof(char));

            if ((*fileInfo + (*fileCount) - 1)->name == NULL)
            {
                perror("Memory allocation error");
                exit(errno);
            }

            if ((*fileInfo + (*fileCount) - 1)->fileName == NULL)
            {
                perror("Memory allocation error");
                exit(errno);
            }

            strncpy((*fileInfo + (*fileCount) - 1)->fileName, dirEl->d_name, strlen(dirEl->d_name));
			strncpy((*fileInfo + (*fileCount) - 1)->name, dirName, strlen(dirName));
			strcat((*fileInfo + (*fileCount) - 1)->name, dirEl->d_name);
            
			struct stat buf;

			if (lstat((*fileInfo + (*fileCount) - 1)->name, &buf) == -1)
            {
                perror("File info get error");
                exit(errno);
            }

			(*fileInfo + (*fileCount) - 1)->size = (int)buf.st_size;

			unsigned char *msg = NULL;
			msg = (unsigned char*)calloc(((int)buf.st_size + 1), sizeof(unsigned char));

            if (msg == NULL)
            {
                perror("Memory allocation error");
                exit(errno);
            }
            
			FILE *file;
			file = fopen((*fileInfo + (*fileCount) - 1)->name, "rb");

            if (options[2] && buf.st_size == 0)
            {
                (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
            }
            else
            {
                if (file != NULL)
                {
                    fread (msg, 1, (int)buf.st_size, file);
                    fclose(file);
                    msg[(int)buf.st_size] = '\0';
                    GetSumMD5((uint8_t*)msg, (size_t)buf.st_size, (*fileInfo + (*fileCount) - 1)->sumMD5);
                }
                else
                {
                    (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                }
            }

            if (options[6] && !(*fileInfo + (*fileCount) - 1)->flagIgnore)
            {
                int nameSize;
                nameSize = strlen(fileName);

                if (fileName[0] == '*' && fileName[nameSize - 1] == '*')
                {
                    char* bufFileName = NULL;
                    bufFileName = (char*)calloc((nameSize - 1), sizeof(char));

                    if (bufFileName == NULL)
                    {
                        perror("Memory allocation error");
                        exit(errno);
                    }

                    for (int j = 1; fileName[j] != '\0'; j++)
                    {
                        bufFileName[j - 1] = fileName[j];
                    }

                    nameSize -= 2;
                    bufFileName[nameSize] = '\0';

                    if (strstr((*fileInfo + (*fileCount) - 1)->fileName, bufFileName) == NULL)
                    {
                        (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                    }

                    free(bufFileName);
                }
                else if (fileName[0] == '*')
                {
                    char* bufFileName = NULL;
                    bufFileName = (char*)calloc((nameSize), sizeof(char));

                    if (bufFileName == NULL)
                    {
                        perror("Memory allocation error");
                        exit(errno);
                    }

                    for (int j = 1; fileName[j] != '\0'; j++)
                    {
                        bufFileName[j - 1] = fileName[j];
                    }

                    nameSize -= 1;
                    bufFileName[nameSize] = '\0';
                    
                    char *pointer;
                    pointer = strstr((*fileInfo + (*fileCount) - 1)->fileName, bufFileName);

                    if (pointer == NULL)
                    {
                        (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                    }
                    else
                    {
                        if (pointer[nameSize] != '\0')
                        {
                            (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                        }
                    }

                    free(bufFileName);
                }
                else if (fileName[nameSize - 1] == '*')
                {
                    char* bufFileName = NULL;
                    bufFileName = (char*)calloc((nameSize), sizeof(char));

                    if (bufFileName == NULL)
                    {
                        perror("Memory allocation error");
                        exit(errno);
                    }

                    for (int j = 0; fileName[j + 1] != '\0'; j++)
                    {
                        bufFileName[j] = fileName[j];
                    }

                    nameSize -= 1;
                    bufFileName[nameSize] = '\0';
                    
                    char *pointer;
                    pointer = strstr((*fileInfo + (*fileCount) - 1)->fileName, bufFileName);

                    if (pointer == NULL)
                    {
                        (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                    }
                    else
                    {
                        if (strlen(pointer) != strlen((*fileInfo + (*fileCount) - 1)->fileName))
                        {
                            (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                        }
                    }

                    free(bufFileName);
                }
                else
                {
                    if (strcmp((*fileInfo + (*fileCount) - 1)->fileName, fileName) != 0)
                    {
                        (*fileInfo + (*fileCount) - 1)->flagIgnore = 1;
                    }
                }
            }

			free(msg);
		}
	}

	GetFilesInfo(dirName, dir, fileInfo, fileCount, options, fileName);
}