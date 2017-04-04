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
  if (entry->vfat || entry->deleted || entry->volumnLabel) {
    return;
  }

  if (entry->size == 0) {
    // directory
    printf("DIR     %13d %11d  %-s\n", entry->startCluster, entry->size, entry->name);
  } else {
    // file
    printf("FILE    %13d %11d  %-s.%-s\n", entry->startCluster, entry->size, entry->name, entry->extension);
  }
}
