#include"GetDuplicateList.h"

void GetDuplicateList(struct FileInfo* fileInfo, int fileCount, int* options, char* saveFilePath, char* fileName)
{
	int flagFirstDuplicate;
	int *duplicateId = NULL;
	int duplicateCount;
	int duplicateSetCount = 0;

	for (int i = 0; i < fileCount; i++)
	{
		flagFirstDuplicate = 1;

		if (!(fileInfo + i)->flagIgnore)
		{
			for (int j = i + 1; j < fileCount; j++)
			{
				if (!(fileInfo + j)->flagIgnore)
				{
					if ((fileInfo + i)->size == (fileInfo + j)->size)
					{
						int cmpFlag = 1;

						for (int k = 0; k < 16; k++)
						{
							if(*(((fileInfo + i)->sumMD5) + k) != *(((fileInfo + j)->sumMD5) + k))
                            {
                                cmpFlag = 0;
                                break;
                            }
						}

						if (cmpFlag)
						{
							unsigned char *msg1 = NULL;
							unsigned char *msg2 = NULL;
							msg1 = (unsigned char*)calloc(((fileInfo + i)->size + 1), sizeof(unsigned char));
							msg2 = (unsigned char*)calloc(((fileInfo + j)->size + 1), sizeof(unsigned char));

                            if (msg1 == NULL)
                            {
                                perror("Memory allocation error");
                                exit(errno);
                            }

                            if (msg2 == NULL)
                            {
                                perror("Memory allocation error");
                                exit(errno);
                            }

							FILE *file;

							file = fopen((fileInfo + i)->name, "rb");

                            if (file == NULL)
                            {
                                perror("File open error");
                                exit(errno);
                            }

							fread (msg1, 1, (fileInfo + i)->size, file);
							fclose(file);
							msg1[(fileInfo + i)->size] = '\0';

							file = fopen((fileInfo + j)->name, "rb");

                            if (file == NULL)
                            {
                                perror("File open error");
                                exit(errno);
                            }

							fread (msg2, 1, (fileInfo + j)->size, file);
							fclose(file);
							msg2[(fileInfo + j)->size] = '\0';

							if (!strcmp(msg1, msg2))
							{
								OutputDuplicateSet(&duplicateCount, &duplicateSetCount, &duplicateId, options, fileInfo, i, j, saveFilePath, &flagFirstDuplicate);

								(fileInfo + j)->flagIgnore = 1;
							}

							free(msg1);
							free(msg2);
						}
					}
				}
			}

			if (flagFirstDuplicate == 0)
			{
				if (options[4])
				{
					char savedFilesIdStr[1000];
					int flagCorrectInput = 0;
					int* savedFilesId;
					int savedFilesCount = 0;

					do
					{
						if (flagCorrectInput)
						{
							printf("\nIncorrect input\n");
						}

						flagCorrectInput = 0;
                        
						printf("\nPreserved files: ");
						fgets(savedFilesIdStr, 999, stdin);

						int v;

						for (v = 0; savedFilesIdStr[v] != '\0'; v++)
						{
							if (savedFilesIdStr[v] != '\n' && (savedFilesIdStr[v] > '9' || savedFilesIdStr[v] < '0') && savedFilesIdStr[v] != ' ')
							{
								flagCorrectInput = 1;
							}
						}

						if (!flagCorrectInput)
						{
							savedFilesIdStr[v - 1] = '\0';
							v--;

							for (int k = 0; k < v; k++)
							{
								if (savedFilesIdStr[k] <= '9' && savedFilesIdStr[k] >= '0')
								{
									savedFilesCount++;

									if (savedFilesCount == 1)
									{
										savedFilesId = (int*)calloc(savedFilesCount, sizeof(int));

                                        if (savedFilesId == NULL)
                                        {
                                            perror("Memory allocation error");
                                            exit(errno);
                                        }
									}
									else
									{
										savedFilesId = (int*)realloc(savedFilesId, savedFilesCount * sizeof(int));

                                        if (savedFilesId == NULL)
                                        {
                                            perror("Memory allocation error");
                                            exit(errno);
                                        }
									}

									savedFilesId[savedFilesCount - 1] = 0;

									for (int f = 0; savedFilesIdStr[k] != '\0' && savedFilesIdStr[k] != ' '; k++, f++)
									{
                                        if (f != 0)
                                        {
                                            savedFilesId[savedFilesCount - 1] *= 10;
                                        }

										savedFilesId[savedFilesCount - 1] += savedFilesIdStr[k] - '0';
									}
								}
							}
						}
					} while(flagCorrectInput);

					printf("\n");

					if (options[1])
					{
						FILE *saveFile;

						saveFile = fopen(saveFilePath, "r+");

                        if (saveFile == NULL)
                        {
                            perror("File open error");
                            exit(errno);
                        }

						fseek(saveFile, 0, 2);
						fprintf(saveFile, "\n");
						fclose(saveFile);
					}

					for (int f = 0; f < duplicateCount; f++)
					{
						int flagSaveFile = 0;

						for (int k = 0; k < savedFilesCount; k++)
						{
							if (f == savedFilesId[k] - 1)
							{
								flagSaveFile = 1;
								break;
							}
						}

						if (flagSaveFile)
						{
							printf("[+] %s\n", (fileInfo + duplicateId[f])->name);
							if (options[1])
							{
								FILE *saveFile;

								saveFile = fopen(saveFilePath, "r+");

                                if (saveFile == NULL)
                                {
                                    perror("File open error");
                                    exit(errno);
                                }

								fseek(saveFile, 0, 2);
								fprintf(saveFile, "[+] %s\n", (fileInfo + duplicateId[f])->name);
								fclose(saveFile);
							}
						}
						else
						{
							printf("[-] %s\n", (fileInfo + duplicateId[f])->name);
							if (options[1])
							{
								FILE *saveFile;
								saveFile = fopen(saveFilePath, "r+");

                                if (saveFile == NULL)
                                {
                                    perror("File open error");
                                    exit(errno);
                                }

								fseek(saveFile, 0, 2);
								fprintf(saveFile, "[-] %s\n", (fileInfo + duplicateId[f])->name);
								fclose(saveFile);
							}
							remove((fileInfo + duplicateId[f])->name);
						}
					}
					free(savedFilesId);
				}
			}

            free(duplicateId);
			duplicateId = NULL;
		}
	}

    printf("\n");
}

void OutputDuplicateSet(int* duplicateCount, int* duplicateSetCount, int** duplicateId, int* options, struct FileInfo* fileInfo, int i, int j, char* saveFilePath, int* flagFirstDuplicate)
{
	if (*flagFirstDuplicate)
	{
		*duplicateCount = 2;
		(*duplicateSetCount)++;
		*duplicateId = (int*)calloc(2, sizeof(int));

		if (*duplicateId == NULL)
		{
			perror("Memory allocation error");
			exit(errno);
		}
		
		duplicateId[0][0] = i;
		duplicateId[0][1] = j;

		printf("\n------Duplicates set %d------\n", *duplicateSetCount);

		if (options[3])
		{
			printf("Duplicate size: %d bytes\n", (fileInfo + i)->size);
		}

		printf("[1] %s\n", (fileInfo + i)->name);
		printf("[2] %s\n", (fileInfo + j)->name);

		if (options[1])
		{
			FILE *saveFile;

			saveFile = fopen(saveFilePath, "r+");

			if (saveFile == NULL)
			{
				perror("File open error");
				exit(errno);
			}

			fseek(saveFile, 0, 2);

			fprintf(saveFile, "\n------Duplicates set %d------\n", *duplicateSetCount);

			if (options[3])
			{
				fprintf(saveFile, "Duplicate size: %d bytes\n", (fileInfo + i)->size);
			}

			fprintf(saveFile, "[1] %s\n", (fileInfo + i)->name);
			fprintf(saveFile, "[2] %s\n", (fileInfo + j)->name);

			fclose(saveFile);
		}

		*flagFirstDuplicate = 0;
	}
	else
	{
		(*duplicateCount)++;
		*duplicateId = (int*)realloc(*duplicateId, *duplicateCount * sizeof(int));

		if (*duplicateId == NULL)
		{
			perror("Memory allocation error");
			exit(errno);
		}

		duplicateId[0][*duplicateCount - 1] = j;

		printf("[%d] %s\n", *duplicateCount, (fileInfo + j)->name);

		if (options[1])
		{
			FILE *saveFile;

			saveFile = fopen(saveFilePath, "r+");

			if (saveFile == NULL)
			{
				perror("File open error");
				exit(errno);
			}

			fseek(saveFile, 0, 2);
			fprintf(saveFile, "[%d] %s\n", *duplicateCount, (fileInfo + j)->name);
			fclose(saveFile);
		}
	}
}