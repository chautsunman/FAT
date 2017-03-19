/*
 * Program to print information about a FAT file system.
 */
#include "fatinfo.h"
#include <stdio.h>
#include <stddef.h>
#include "directory.h"
#include "disk.h"

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


  print_filesystem_info(&fsInfo);
  
  
  // Add calls to function to print the directory tree

  return 0;
}
