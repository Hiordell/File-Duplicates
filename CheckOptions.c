#include"CheckOptions.h"

void CheckOptions(int argc, char** argv, char** dirName, int* options, char** fileName, char** saveFilePath)
{

	int flagNoPath = 1;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && strlen(argv[i]) == 2)
		{
			if (flagNoPath == 0)
			{
				printf("\nInvalid command line arguments\n\n");
				exit(0);
			}

			char option = argv[i][1];
            
			switch(option)
			{
				case 'r':
				{
					options[0] = 1;
					break;
				}

				case 'n':
				{
					options[2] = 1;
					break;
				}

				case 's':
				{
					options[3] = 1;
					break;
				}

				case 'd':
				{
					options[4] = 1;
					break;
				}

				case 'h':
				{
					options[5] = 1;
					break;
				}

				case 'N':
				{
					options[6] = 1;
					i++;

					if (i == argc)
					{
						printf("\nInvalid command line arguments\n\n");
						exit(0);
					}

					*fileName = (char*)calloc((strlen(argv[i]) + 1), sizeof(char));

                    if (*fileName == NULL)
                    {
                        perror("Memory allocation error");
                        exit(errno);
                    }

                    strncpy(*fileName, argv[i], strlen(argv[i]));

                    for (int j = 0; fileName[0][j] != '\0'; j++)
                    {
                        if (fileName[0][j] == '/')
                        {
                            printf("\nInvalid file name\n\n");
						    exit(0);
                        }
                    }

					int nameSize;
					nameSize = strlen(*fileName);

					if (fileName[0][0] == '*' && nameSize == 1)
					{
						printf("\nInvalid file name\n\n");
						exit(0);
					}

					if (fileName[0][0] == '*' && fileName[0][nameSize - 1] == '*' && nameSize == 2)
					{
						printf("\nInvalid file name\n\n");
						exit(0);
					}
					
					break;
				}

				case 'f':
				{
					options[1] = 1;
					i++;

					if (i == argc)
					{
						printf("\nInvalid command line arguments\n\n");
						exit(0);
					}

					if (strlen(argv[i]) < 5)
					{
						printf("\nInvalid command line arguments\n\n");
						exit(0);
					}

                    *saveFilePath = (char*)calloc((strlen(argv[i]) + 1), sizeof(char));

                    if (*saveFilePath == NULL)
                    {
                        perror("Memory allocation error");
                        exit(errno);
                    }

                    strncpy(*saveFilePath, argv[i], strlen(argv[i]));

					int j;
                    j = strlen(*saveFilePath);

					if (saveFilePath[0][j - 1] != 't' && saveFilePath[0][j - 2] != 'x' && saveFilePath[0][j - 3] != 't' && saveFilePath[0][j - 4] != '.')
					{
						printf("\nInvalid save file path\n\n");
						exit(0);
					}

					FILE *file;
					file = fopen(*saveFilePath, "w");

					if (file == NULL)
					{
						perror("File open error");
						exit(errno);
					}

					fclose(file);
					break;
				}

				default:
				{
					printf("\nInvalid command line arguments\n\n");
					exit(0);
				}
			}
		}
		else
		{
            *dirName = (char*)calloc((strlen(argv[i]) + 1), sizeof(char));

            if (*dirName == NULL)
            {
                perror("Memory allocation error");
                exit(errno);
            }

            strncpy(*dirName, argv[i], strlen(argv[i]));

			if (dirName[0][strlen(*dirName) - 1] != '/')
			{
				int nameSize;
				nameSize = strlen(*dirName);
				*dirName = (char*)realloc(*dirName, (nameSize + 2) * sizeof(char));

				if (*dirName == NULL)
				{
					perror("Memory allocation error");
					exit(errno);
				}

				dirName[0][nameSize] = '/';
				dirName[0][nameSize + 1] = '\0';
			}

			flagNoPath = 0;
		}
	}

	if (flagNoPath)
    {
		*dirName = (char*)calloc(3, sizeof(char));

		if (*dirName == NULL)
		{
			perror("Memory allocation error");
			exit(errno);
		}

		strncpy(*dirName, "./", 2);
    }
}