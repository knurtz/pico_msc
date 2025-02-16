#ifndef PICO_MSC_FAT_H
#define PICO_MSC_FAT_H

#include "pico/stdlib.h"

//todo: merge with pico_msc.h

bool FAT_Init(void);
void FAT_ListFolder(const char* dir_name, uint8_t current_depth);
void FAT_PrintFile(const char* filename, uint32_t len);
unsigned int FAT_ReadFileToBuffer(const char* filename, uint32_t offset, uint32_t len, uint8_t* buffer);
uint32_t FAT_GetFilesize(const char* filename);

#endif /* PICO_MSC_FAT_H */
