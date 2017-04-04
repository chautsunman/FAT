/*
 * fat12.h
 *
 * Functions used to deal with the file allocation table.
 */
#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>

int lastClusterFat12(uint8_t *, int);

int nextClusterFat12(uint8_t *, int);

#endif
