#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "disk.h"

// This file contains a number of functions to simulate a disk drive.
// To use these routine you fist need to call the openDisk() function with the 
// name of the file containing the disk image. Note: this code is not 
// re-entrant or thread safe and you can only have 1 disk open at at time. 
// 
//
// To retrieve data from "disk" calls to readSectors() are made. This 
// function takes the sector number to read, the number of sectors, and 
// a buffer to put the data read from the disk into. Sector numbering starts
// at 0. 
// 
// These routines assume an initial DEFAULT_SECTOR_SIZE of 512 bytes. The
// first 512 bytes contain the boot block/sector information. As part of that
// information will be the disk's actual sector size. Once the sector size
// is known a call to setSectorSize() needs to be made to set the actual
// sector size of the disk. This sector size will be used for subsequent 
// readSectors() calls.
//
//


// Static variables to maintain state information needed to manage the 
// disk. Only one disk can be open and in use at time. A 
//


static uint8_t *   startLocation = NULL;
static uint8_t *   lastByte = NULL;
static int         sectorSize = DEFAULT_SECTOR_SIZE;
static struct stat statBuff;


// Opens the file containing the disk image and gets things ready
// for subsequent reads. It takes the name of the file containing the
// disk image.
// 
// Return values
//
//  0 --> success
// -1 --> Failure - file could not be opened
// -2 --> Failure - couldn't stat the file so it can't be used
// -3 --> Failure - couldn't map the file into the process address space
// 
int openDisk(char *fname) {

  int fd;

  // Open file and check that it opened 
  fd = open(fname, O_RDONLY);
  
  if (fd < 0) 
    return -1;

  // Stat the file so that we can get basic information about the file
  // Assuming there are no bugs int this code, the call should
  // never fail.

  if (fstat(fd, &statBuff)) {
    close(fd);
    return -2;
  }


  // Map the file into the processes memory, and remember 
  // where it was put.

  startLocation = mmap(0, statBuff.st_size, PROT_READ, 
		       MAP_FILE | MAP_PRIVATE, fd, 0);

  // Mapping failed
  if (startLocation == NULL) {
    close(fd);
    return -3;
  }

  // Compute the address of the last byte of the file. Note
  // that 1 is subtract because of 0 based counting for addresses. 
  lastByte = startLocation + statBuff.st_size - 1;

  return 0;
}



/* 
 * This function reads an integer number of sectors into a passed in buffer. 
 * The returned value is the number of bytes transfered. This value is 0 if 
 * a request is made to start a read beyond the end of the "disk" and -1
 * if things haven't been configured yet.
 *  
 *   sectorStart  - the sector number to start the read at
 *   numberOfSectors - the number of sectors to read.
 *   buff            - the address of buffer location to 
 *                     copy the bytes to. 
 */


int readSectors(int sectorStart, int numberOfSectors, uint8_t *buff) {
  
  // Make sure the disk has been opened
  if (!startLocation)
    return -1;

  // How much to read and where to start reading from

  int sizeToRead = numberOfSectors * sectorSize;      
  uint8_t *start = sectorStart * sectorSize + startLocation;



  // Check that the request doesn't start beyond the end of the disk.
  if (start > lastByte) 
    return 0;

  // Check that the request doesn't go beyond the end of the disk. 
  // If it does shorten the read to go just to the end of disk.
  uint8_t *lastByteToRead = start + sizeToRead - 1 ;
  if (lastByteToRead > lastByte) {
    perror("BUG");
    sizeToRead = lastByte - start + 1;
  }

  //  printf("%d  %u %u %d\n", sectorStart, start, lastByte, sizeToRead);
  // This will always work or crash :-) 
  memmove(buff, start, sizeToRead);
  return sizeToRead;  
  
}

// Set the sector size to ssize

int setSectorSize(int ssize) {
    return sectorSize = ssize;
}


// return the size of the sector being used.
int getSectorSize(void) {
  return sectorSize;
}



