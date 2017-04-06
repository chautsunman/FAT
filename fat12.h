/*
 * fat12.h
 *
 * Functions used to deal with the file allocation table.
 */
#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>

// checks if the cluster is the last cluster
int lastClusterFat12(const uint8_t *fat, const int cluster);

// returns the next cluster
int nextClusterFat12(const uint8_t *fat, const int cluster);

#endif
