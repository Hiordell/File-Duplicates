#ifndef GETSUMMD5_H
#define GETSUMMD5_H

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdint.h>

void ToBytes(uint32_t, uint8_t*);
uint32_t ToInt32(const uint8_t*);
void GetSumMD5(const uint8_t*, size_t, uint8_t*);

#endif