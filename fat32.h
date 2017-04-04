/*
 * fat32.h
 *
 * Functions used to deal with the file allocation table.
 */
#ifndef FAT32_H
#define FAT32_H


#include "fat12.h"


// Any FAT 32 specific data structures go here as well as function prototypes. Some examples
// FAT32 specific code might be the code to determine the next cluster from a FAT table entry.
char * getClusterChain(uint8_t *, int, int);

int lastClusterFat32(uint8_t *, int);

int nextClusterFat32(uint8_t *, int);

#endif
