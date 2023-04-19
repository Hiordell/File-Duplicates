#ifndef GETDUPLICATELIST_H
#define GETDUPLICATELIST_H

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

void GetDuplicateList(struct FileInfo*, int, int*, char*, char*);

void OutputDuplicateSet(int*, int*, int**, int*, struct FileInfo*, int, int, char*, int*);

#endif