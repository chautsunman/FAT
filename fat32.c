/*
 * fat32.c
 *
 */
#include <stdio.h>
#include <string.h>

#include "fat32.h"

char * getClusterChain(const uint8_t *fat, const int cluster, const int sectorBits) {
  char clusterChain[128] = "";
  int currentCluster = cluster;
  int clusterRangeLow = cluster;
  int clusterRangeHigh = cluster - 1;
  char nextClusterChain[16];

  // set functions to check if the last cluster and get the next cluster
  int (*lastClusterFunc)(const uint8_t *, const int) = lastClusterFat32;
  int (*nextClusterFunc)(const uint8_t *, const int) = nextClusterFat32;
  if (sectorBits == 12) {
    lastClusterFunc = lastClusterFat12;
    nextClusterFunc = nextClusterFat12;
  }

  if (lastClusterFunc(fat, currentCluster)) {
    // one-cluster file or directory
    sprintf(nextClusterChain, "%d,", currentCluster);

    strcat(clusterChain, nextClusterChain);
  } else {
    // search for clusters
    while (!lastClusterFunc(fat, currentCluster)) {
      if (currentCluster != clusterRangeHigh + 1) {
        // not continuous, print the last range
        if (clusterRangeHigh == clusterRangeLow) {
          // print the last sector
          sprintf(nextClusterChain, "%d,", clusterRangeHigh);
        } else {
          // print the last range
          sprintf(nextClusterChain, "%d-%d,", clusterRangeLow, clusterRangeHigh);
        }

        strcat(clusterChain, nextClusterChain);

        // set the start of the new range
        clusterRangeLow = currentCluster;
      }

      // set the end of the current range
      clusterRangeHigh = currentCluster;

      // get the next cluster
      currentCluster = nextClusterFunc(fat, currentCluster);
    }

    // print the last clusters
    if (currentCluster != clusterRangeHigh + 1) {
      // not continuous, print the last range and the current cluster
      if (clusterRangeHigh == clusterRangeLow) {
        // print the last sector and the current sector
        sprintf(nextClusterChain, "%d,%d,", clusterRangeHigh, currentCluster);
        } else {
        // print the last range and the current sector
        sprintf(nextClusterChain, "%d-%d,%d,", clusterRangeLow, clusterRangeHigh, currentCluster);
      }
    } else {
      // continuous, print the range
      sprintf(nextClusterChain, "%d-%d,", clusterRangeLow, currentCluster);
    }

    strcat(clusterChain, nextClusterChain);
  }

  // append "[END]"
  strcat(clusterChain, "[END]");

  return clusterChain;
}

int lastClusterFat32(const uint8_t *fat, const int cluster) {
  return (fat[cluster*4+3] * 16777216 + fat[cluster*4+2] * 65536 + fat[cluster*4+1] * 256 + fat[cluster*4] > 0x0FFFFFF8);
}

int nextClusterFat32(const uint8_t *fat, const int cluster) {
  return (fat[cluster*4+3] * 16777216 + fat[cluster*4+2] * 65536 + fat[cluster*4+1] * 256 + fat[cluster*4]);
}
