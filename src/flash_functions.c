#include <stdlib.h>
#include <stdio.h>

#include "flash_functions.h"

#include "tusb.h"
#include "msc_disk.h"

#include "pico/flash.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// Keep the first 2 sections of 8 sectors permanently in RAM and only write to flash periodically and on eject.
// These sections contain FAT and root directory and will be read and overwritten many times.
uint8_t flash_start[16][512];
uint8_t flash_start_modified = 0;      // 0 - no changes, 1 - first 8 sectors changed, 2 - second 8 sectors changed, 3 - both changed

// Another variable holds a copy of the section that has been modified last.
// Only once a different section is accessed will the flash be actually written.
uint8_t flash_section[8][512];
uint32_t current_section = 0;
uint8_t modified_sectors = 0;

// If any changes remain, they are periodically written to flash
uint32_t last_write_time_ms = 0;
uint16_t write_interval_ms = 10000;

bool disk_initialized = false;

void Flash_Init(void) {
    // Initialize the start section
    memcpy(flash_start, msc_disk, sizeof(flash_start));
    disk_initialized = true;
}

uint32_t Flash_ReadQueued(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    if (offset != 0 || bufsize != 512) return 0;

    uint32_t target_section = lba / 8;
    uint8_t target_sector = lba % 8;

    // Get data for first 2 sections from flash_start[] in RAM
    if (target_section < 2) {
        memcpy(buffer, flash_start[target_section * 8 + target_sector], bufsize);
    }

    // Get data from flash_section[] (also in RAM), if modified sector has not yet been written to flash
    else if (target_section == current_section && modified_sectors & (1 << target_sector)) {
        memcpy(buffer, flash_section[target_sector], bufsize);
    }

    // Get all other sectors from flash
    else {
        memcpy(buffer, msc_disk[lba], bufsize);
    }

    return bufsize;
}

uint32_t Flash_WriteQueued(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    if (offset != 0 || bufsize != 512) return 0;

    uint32_t target_section = lba / 8;
    uint8_t target_sector = lba % 8;

    if (target_section < 2) {            
        //for (uint32_t start_addr = lba * 512 + offset; start_addr < lba * 512 + bufsize; start_addr += 512)

        memcpy(flash_start[target_section * 8 + target_sector], buffer, bufsize);
        flash_start_modified |= 1 << target_section;

        return bufsize;
    } 
    else {
        // If data needs to be written to a different section than last time, now is the time to push the pending changes to flash
        if (target_section != current_section) {
            Flash_WriteCurrentSection();
        }

        current_section = target_section;
        memcpy(flash_section[target_sector], buffer, bufsize);
        modified_sectors |= 1 << target_sector;

        return bufsize;
    }
}

static void call_flash_routine(void* param) {
    uint32_t section = ((uint32_t)param);
    uint8_t* source = section < 2 ? (uint8_t*)(flash_start[section * 8]) : (uint8_t*)(flash_section);

    flash_range_erase((uint32_t)(msc_disk[section * 8]) - XIP_BASE, 8 * 512);
    flash_range_program((uint32_t)(msc_disk[section * 8]) - XIP_BASE, source, 8 * 512);
}

void Flash_WriteStartSection(void) {
    if (!flash_start_modified) return;

    if (flash_start_modified & 0b01) {
        flash_safe_execute(call_flash_routine, (void*) 0, UINT32_MAX);
    }
    if (flash_start_modified & 0b10) {
        flash_safe_execute(call_flash_routine, (void*) 1, UINT32_MAX);
    }

    flash_start_modified = 0;
}

void Flash_WriteCurrentSection(void) {
    if (!modified_sectors) return;

    // Fill in all sectors that haven't been modified, as they will be overwritten
    for (uint8_t sector = 0; sector < 8; sector++) {
        if (!(modified_sectors & (1 << sector))) {
            memcpy(flash_section[sector], msc_disk[current_section * 8 + sector], 512);
        }
    }

    flash_safe_execute(call_flash_routine, (void*) current_section, UINT32_MAX);
    modified_sectors = 0;
}

// Should be called periodically
void Flash_WriteCycle(bool forced) {
    if (!forced && time_us_32() / 1000 < last_write_time_ms + write_interval_ms) return;
    last_write_time_ms = time_us_32() / 1000;

    Flash_WriteStartSection();
    Flash_WriteCurrentSection();
}