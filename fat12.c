/*
 * fat12.c
 *
 */
#include <stdio.h>

#include "fat12.h"
// Add code specific to fat12 manipulation. Since some of the fat32 structures are the same as
// fat12, some (all?) of these functions might be used by fat32.
int lastClusterFat12(uint8_t *fat, int cluster) {
  int fatEntriesPair = fat[(cluster/2)*3+2] * 65536 + fat[(cluster/2)*3+1] * 256 + fat[(cluster/2)*3];

  return (cluster % 2 == 0) ? (fatEntriesPair % 4096 == 0xFFF) : (fatEntriesPair / 4096 == 0xFFF);
}

int nextClusterFat12(uint8_t *fat, int cluster) {
  int fatEntriesPair = fat[(cluster/2)*3+2] * 65536 + fat[(cluster/2)*3+1] * 256 + fat[(cluster/2)*3];

  return (cluster % 2 == 0) ? fatEntriesPair % 4096 : fatEntriesPair / 4096;
}
