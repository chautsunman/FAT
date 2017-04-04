/*
 * fat32.c
 *
 */
#include <stdio.h>
#include <string.h>

#include "fat32.h"

char * getClusterChain(uint8_t *fat, int cluster, int sectorBits) {
  char clusterChain[128] = "";
  int currentCluster = cluster;
  int clusterRangeLow = cluster;
  int clusterRangeHigh = cluster - 1;
  char nextClusterChain[16];

  int (*lastClusterFunc)(uint8_t *, int) = lastClusterFat32;
  int (*nextClusterFunc)(uint8_t *, int) = nextClusterFat32;

  if (sectorBits == 12) {
    lastClusterFunc = lastClusterFat12;
    nextClusterFunc = nextClusterFat12;
  }

  if (lastClusterFunc(fat, currentCluster)) {
    sprintf(nextClusterChain, "%d,", currentCluster);

    strcat(clusterChain, nextClusterChain);
  } else {
    while (!lastClusterFunc(fat, currentCluster)) {
      if (currentCluster != clusterRangeHigh + 1) {
        if (clusterRangeHigh == clusterRangeLow) {
          sprintf(nextClusterChain, "%d,", clusterRangeHigh);
        } else {
          sprintf(nextClusterChain, "%d-%d,", clusterRangeLow, clusterRangeHigh);
        }

        strcat(clusterChain, nextClusterChain);

        clusterRangeLow = currentCluster;
      }

      clusterRangeHigh = currentCluster;

      currentCluster = nextClusterFunc(fat, currentCluster);
    }

    if (currentCluster != clusterRangeHigh + 1) {
      if (clusterRangeHigh == clusterRangeLow) {
        sprintf(nextClusterChain, "%d,%d,", clusterRangeHigh, currentCluster);
      } else {
        sprintf(nextClusterChain, "%d-%d,%d,", clusterRangeLow, clusterRangeHigh, currentCluster);
      }
    } else {
      if (clusterRangeHigh == clusterRangeLow) {
        sprintf(nextClusterChain, "%d,", clusterRangeHigh+1);
      } else {
        sprintf(nextClusterChain, "%d-%d,", clusterRangeLow, clusterRangeHigh+1);
      }
    }

    strcat(clusterChain, nextClusterChain);
  }

  strcat(clusterChain, "[END]");

  return clusterChain;
}

int lastClusterFat32(uint8_t *fat, int cluster) {
  return (fat[cluster*4+3] * 16777216 + fat[cluster*4+2] * 65536 + fat[cluster*4+1] * 256 + fat[cluster*4] > 0x0FFFFFF8);
}

int nextClusterFat32(uint8_t *fat, int cluster) {
  return (fat[cluster*4+3] * 16777216 + fat[cluster*4+2] * 65536 + fat[cluster*4+1] * 256 + fat[cluster*4]);
}
