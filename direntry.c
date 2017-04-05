/*
 * direntry.c
 *
 * Functions used to deal with directory entries.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "direntry.h"

void printDirectoryEntry(const struct DirectoryEntry *entry) {
  if (entry->size == 0) {
    // directory
    printf("DIR     %13d %11d  %-s -> %s\n", entry->startCluster, entry->size, entry->name, entry->clusterChain);
  } else {
    // file
    printf("FILE    %13d %11d  %-s.%-s -> %s\n", entry->startCluster, entry->size, entry->name, entry->extension, entry->clusterChain);
  }
}

void trimString(char *str) {
  char *c;

  for (c = str; *c != 0; c++) {
    if (*c == 32) {
      char *d;
      int end = 1;

      for (d = c; *d != 0; d++) {
        if (*d != 32) {
          end = 0;
          break;
        }
      }

      if (end) {
        *c = 0;
        return;
      }
    }
  }
}
