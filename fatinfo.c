/*
 * Program to print information about a FAT file system.
 */
#include "fatinfo.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "directory.h"
#include "disk.h"


void parseFileSystemInfo(uint8_t *, filesystem_info *);


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


  // Add calls to function to print the directory tree

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
  } else {
    fsInfo->fs_type = FAT12;
    fsInfo->sector_size = bootSector[12] * 256 + bootSector[11];
    fsInfo->cluster_size = bootSector[13];
    fsInfo->rootdir_size = bootSector[18] * 256 + bootSector[17];
    fsInfo->sectors_per_fat = bootSector[23] * 256 + bootSector[22];
    fsInfo->reserved_sectors = bootSector[15] * 256 + bootSector[14];
    fsInfo->hidden_sectors = bootSector[29] * 256 + bootSector[28];
  }
}
