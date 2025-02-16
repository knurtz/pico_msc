#ifndef FLASH_FUNCTIONS_H
#define FLASH_FUNCTIONS_H

#include "pico/stdlib.h"

uint32_t Flash_ReadQueued(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
uint32_t Flash_WriteQueued(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);

bool Flash_WriteStartSection(void);
bool Flash_WriteCurrentSection(void);

#endif /* FLASH_FUNCTIONS_H */
