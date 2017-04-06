/*
 * Program to print information about a FAT file system.
 */
#include "fatinfo.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "directory.h"
#include "disk.h"
#include "direntry.h"


void parseFileSystemInfo(uint8_t *bootSector, filesystem_info *fsInfo);
void printDirectoryTree(
    const uint8_t *fat,
    const unsigned int dirSector, const unsigned int dirSectorSize,
    const unsigned int sectorSize, const unsigned int clusterSize,
    const unsigned int clusterOffset,
    const char *parentPath, const FS_TYPE fsType,
    const int rootDir);

int getBit(uint8_t byte, int i);


/*
 * Function to print information about a FAT filesystem (useful for debugging).
 */
void print_filesystem_info(const filesystem_info *fsinfo)
{
    printf ("Sector size: %zu\n", fsinfo->sector_size);
    printf ("Cluster size in sectors: %u\n", fsinfo->cluster_size);
    printf ("Root directory size (nb of entries): %u\n", fsinfo->rootdir_size);

    printf ("Sectors per fat: %u\n", fsinfo->sectors_per_fat);
    printf ("Reserved sectors: %u\n", fsinfo->reserved_sectors);
    printf ("Hidden sectors: %u\n", fsinfo->hidden_sectors);

    printf ("Fat offset in sectors: %u\n", fsinfo->fat_offset);
    printf ("Root directory offset in sectors: %u\n", fsinfo->rootdir_offset);
    printf ("First cluster offset in sectors: %u\n", fsinfo->cluster_offset);
}

/*
 * Main function.
 */
int main(int argc, char *argv[]) {

  filesystem_info fsInfo;
  // Check for disk image to open

  if (argc != 2)  {
    fprintf(stderr, "Usage: %s diskImage\n", argv[0]);
    return 0;
  }

  // Open the disk image and get things ready to go.

  if (openDisk(argv[1]) < 0) {
    char buff[256];
    snprintf(buff, 256, "Problem opening the disk image file: %s", argv[1]);
    perror(buff);
    return 0;
  }

  // Read the first sector of the disk to get the basic information abou the
  // filesystem on this disk.

  uint8_t  buffer[DEFAULT_SECTOR_SIZE];
  if (readSectors(0, 1, buffer) != DEFAULT_SECTOR_SIZE) {
    char buff[256];
    snprintf(buff, 256, "Problem reading first sector from: %s", argv[1]);
    perror(buff);
    return 0;
  }

  // Make a call to Interpret the boot sector and fill in the  filesystem_info structure);

  // Add your code
  parseFileSystemInfo(buffer, &fsInfo);

  print_filesystem_info(&fsInfo);

  setSectorSize(fsInfo.sector_size);


  // Add calls to function to print the directory tree
  printf("\n\n");
  printf("Type    Start Cluster        Size  Name  -> Cluster Chain\n");
  printf("=================================================================\n");

  uint8_t fat[fsInfo.sector_size*fsInfo.sectors_per_fat];

  if (readSectors(fsInfo.fat_offset, fsInfo.sectors_per_fat, fat) != fsInfo.sector_size*fsInfo.sectors_per_fat) {
    return 1;
  }

  printDirectoryTree(
      fat,
      fsInfo.rootdir_offset, 32 * fsInfo.rootdir_size / getSectorSize(),
      getSectorSize(), fsInfo.cluster_size,
      fsInfo.cluster_offset,
      "", fsInfo.fs_type,
      (fsInfo.fs_type != FAT32));

  return 0;
}


void parseFileSystemInfo(uint8_t *bootSector, filesystem_info *fsInfo) {
  if (bootSector[17] == 0 && bootSector[18] == 0) {
    fsInfo->fs_type = FAT32;
    fsInfo->sector_size = bootSector[12] * 256 + bootSector[11];
    fsInfo->cluster_size = bootSector[13];
    fsInfo->rootdir_size = bootSector[18] * 256 + bootSector[17];
    fsInfo->sectors_per_fat = bootSector[39] * 16777216 + bootSector[38] * 65536 + bootSector[37] * 256 + bootSector[36];
    fsInfo->reserved_sectors = bootSector[15] * 256 + bootSector[14];
    fsInfo->hidden_sectors = bootSector[31] * 16777216 + bootSector[30] * 65536 + bootSector[29] * 256 + bootSector[28];
    fsInfo->fat_offset = fsInfo->reserved_sectors;
    fsInfo->cluster_offset = fsInfo->fat_offset + fsInfo->sectors_per_fat * bootSector[16];
    fsInfo->rootdir_offset = fsInfo->cluster_offset + ((bootSector[47] * 16777216 + bootSector[46] * 65536 + bootSector[45] * 256 + bootSector[44]) - 2) * fsInfo->cluster_size;
  } else {
    fsInfo->fs_type = FAT12;
    fsInfo->sector_size = bootSector[12] * 256 + bootSector[11];
    fsInfo->cluster_size = bootSector[13];
    fsInfo->rootdir_size = bootSector[18] * 256 + bootSector[17];
    fsInfo->sectors_per_fat = bootSector[23] * 256 + bootSector[22];
    fsInfo->reserved_sectors = bootSector[15] * 256 + bootSector[14];
    fsInfo->hidden_sectors = bootSector[29] * 256 + bootSector[28];
    fsInfo->fat_offset = fsInfo->reserved_sectors;
    fsInfo->rootdir_offset = fsInfo->fat_offset + fsInfo->sectors_per_fat * bootSector[16];
    fsInfo->cluster_offset = fsInfo->rootdir_offset + fsInfo->rootdir_size * 32 / fsInfo->sector_size;
  }
}


void printDirectoryTree(
    const uint8_t *fat,
    const unsigned int dirSector, const unsigned int dirSectorSize,
    const unsigned int sectorSize, const unsigned int clusterSize,
    const unsigned int clusterOffset,
    const char *parentPath, const FS_TYPE fsType,
    const int rootDir) {
  // printf("dirSector = %d, dirSectorSize = %d, sectorSize = %d, clusterSize = %d, clusterOffset = %d, parentPath = %s\n", dirSector, dirSectorSize, sectorSize, clusterSize, clusterOffset, parentPath);

  int currentSector = dirSector;
  int sectorsToRead = (dirSectorSize != 0) ? dirSectorSize : clusterSize;
  int currentCluster = ((currentSector - clusterOffset) / clusterSize) + 2;
  int nextCluster = currentCluster;
  int nextSector = currentSector;

  unsigned int numberOfEntries = sectorsToRead * sectorSize / 32;
  uint8_t buf[sectorSize * sectorsToRead];

  // set functions to check if the last cluster and get the next cluster
  int (*lastClusterFunc)(const uint8_t *, const int) = lastClusterFat32;
  int (*nextClusterFunc)(const uint8_t *, const int) = nextClusterFat32;
  if (fsType == FAT12) {
    lastClusterFunc = lastClusterFat12;
    nextClusterFunc = nextClusterFat12;
  }

  // read clusters
  do {
    currentCluster = nextCluster;
    currentSector = nextSector;
    // printf("currentCluster = %d, currentSector = %d, sectorsToRead = %d\n", currentCluster, currentSector, sectorsToRead);

    // read sectors, number of sectors per fat (clusterSize) except root directory
    if (readSectors(currentSector, sectorsToRead, buf) != (sectorSize * sectorsToRead)) {
      exit(1);
    }

    int i = 0;
    // read directory entries until the end of the directory or the end of the cluster
    while (buf[i*32] != 0 && i != numberOfEntries) {
      // skip long directory names, deleted entries, volumn labels, and current and parent entries
      if (buf[i*32+11] == 0x0F || buf[i*32] == 0xE5 || buf[i*32+11] == 0x28 || buf[i*32] == 0x2E) {
        i++;
        continue;
      }

      struct DirectoryEntry entry;

      // parse the directory entry
      entry.startCluster = buf[i*32+27] * 256 + buf[i*32+26];
      entry.size = buf[i*32+31] * 16777216 + buf[i*32+30] * 65536 + buf[i*32+29] * 256 + buf[i*32+28];
      entry.name = malloc(9);
      entry.extension = malloc(4);
      memcpy(entry.name, buf+i*32, 8);
      memcpy(entry.extension, buf+i*32+8, 3);
      entry.name[8] = 0;
      entry.extension[3] = 0;
      trimString(entry.name);
      trimString(entry.extension);

      // get the cluster chain
      entry.clusterChain = getClusterChain(fat, entry.startCluster, fsType);

      // print the directory entry
      printDirectoryEntry(&entry, parentPath);

      // print the sub-directories recursively with depth-first search
      if (entry.size == 0) {
        // parse the new parent path
        char newParentPath[strlen(parentPath)+strlen(entry.name)+1+1];
        strcpy(newParentPath, parentPath);
        strcat(newParentPath, entry.name);
        strcat(newParentPath, "/");

        // print the sub-directories recursively
        printDirectoryTree(
            fat,
            (entry.startCluster - 2) * clusterSize + clusterOffset, clusterSize,
            sectorSize, clusterSize,
            clusterOffset,
            newParentPath, fsType,
            0);
      }

      i++;
    }

    // get the next cluster
    if (!rootDir) {
      nextCluster = nextClusterFunc(fat, currentCluster);
      nextSector = (nextCluster - 2) * clusterSize + clusterOffset;
    }
  } while (!rootDir && !lastClusterFunc(fat, currentCluster));
}


int getBit(uint8_t byte, int i) {
  return (byte >> (7-i)) & 1;
}
