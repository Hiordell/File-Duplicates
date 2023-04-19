#ifndef CHECKOPTIONS_H
#define CHECKOPTIONS_H

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdint.h>

void CheckOptions(int, char**, char**, int*, char**, char**);

#endif