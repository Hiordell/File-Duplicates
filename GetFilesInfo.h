#ifndef GETFILESINFO_H
#define GETFILESINFO_H

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
#include"GetSumMD5.h"

void GetFilesInfo(char*, DIR*, struct FileInfo**, int*, int*, char*);

#endif