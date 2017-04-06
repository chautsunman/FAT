/*
 * direntry.c
 *
 * Functions used to deal with directory entries.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "direntry.h"

void printDirectoryEntry(const struct DirectoryEntry *entry, const char *parentPath) {
  if (entry->size == 0) {
    // directory
    printf("DIR     %13d %11d  %-s%-s -> %s\n", entry->startCluster, entry->size, parentPath, entry->name, entry->clusterChain);
  } else {
    // file
    printf("FILE    %13d %11d  %-s%-s%-s%-s -> %s\n", entry->startCluster, entry->size, parentPath, entry->name, strlen(entry->extension) ? "." : "", entry->extension, entry->clusterChain);
  }
}

void trimString(char *str) {
  char *c;

  for (c = str; *c != 0; c++) {
    // find a space
    if (*c == 32) {
      char *d;
      int end = 1;

      // check if it is the start of the trailing spaces
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
