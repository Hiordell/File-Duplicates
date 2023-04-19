#ifndef FILEINFO_H
#define FILEINFO_H

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdint.h>

struct FileInfo
{
	char *name;
	char *fileName;
	uint8_t sumMD5[16];
	int size;
	int flagIgnore;
};

#endif