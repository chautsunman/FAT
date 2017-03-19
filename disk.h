
#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#define DEFAULT_SECTOR_SIZE  512

int openDisk(char * fname);

int readSectors(int sectorStart, int numberOfSectors, uint8_t *buff); 

int setSectorSize(int ssize);

int getSectorSize(void);

#endif
