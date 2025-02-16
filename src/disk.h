#include "pico/stdlib.h"

// todo: turn disk size into user parameter
#define DISK_SECTOR_NUM         7936       // Disk size 3968k -> 7936 * 512 bytes

#define DISK_SECTOR_SIZE        512
#define DISK_CLUSTER_SIZE       8           // Cluster size 4096 bytes

#define MASS_STORAGE_START      0x10020000

extern const __attribute__((section(".mass_storage")))
uint8_t msc_disk[DISK_SECTOR_NUM][DISK_SECTOR_SIZE];