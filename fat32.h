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
// gets the cluster chain
char * getClusterChain(const uint8_t *fat, const int cluster, const int sectorsBit);

// checks if the cluster is the last cluster
int lastClusterFat32(const uint8_t *fat, const int cluster);

// returns the next cluster
int nextClusterFat32(const uint8_t *fat, const int cluster);

#endif
