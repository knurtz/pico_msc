#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "fat.h"

FATFS fs;
bool fs_mounted = false;

// Global file object for continuous read operations
FIL persistent_file;

bool FAT_Init(void) {    
    FRESULT res;

    if (fs_mounted) return true;

    res = f_mount(&fs, "", 1);
    if (res == FR_OK) {
        char vol_name[10];
        f_getlabel("", vol_name, NULL);
        printf("Mounted %s\n", vol_name);
        fs_mounted = true;
        return true;
    }

    else printf("mount error %d\n", res);
    return false;
}

void FAT_ListFolder(const char* dir_name, uint8_t current_depth) {
    FRESULT res;    
    DIR dir;
    FILINFO fileinfo;

    if (!FAT_Init()) return;

    res = f_opendir(&dir, dir_name);
    if (res) {
        printf("opendir error %d\n", res);
        return;
    }

    res = f_readdir(&dir, &fileinfo);
    if (res) {
        printf("readdir error %d\n", res);
        return;
    }

    while (fileinfo.fname[0]) {
        printf("%*s%s\n", current_depth * 2, "", fileinfo.fname);
        if (fileinfo.fattrib & AM_DIR) {
            size_t next_dir_length = strlen(dir_name) + strlen(fileinfo.altname) + 3;
            char next_dir[next_dir_length];
            snprintf(next_dir, next_dir_length - 1, "%s/%s", dir_name, fileinfo.altname);
            FAT_ListFolder(next_dir, current_depth + 1);
        }
        res = f_readdir(&dir, &fileinfo);
        if (res) {
            printf("readdir error %d\n", res);
            return;
        }
    }
}

void FAT_PrintFile(const char* filename, uint32_t len) {
    FIL f;
    FRESULT res;
    UINT br;
    
    if (!FAT_Init()) return;

    res = f_open(&f, filename, FA_READ);
    if (res) printf("open error %d\n", res);
    else {
        char read_text[len + 1];
        f_read(&f, read_text, len, &br);
        read_text[br] = '\0';
        printf("%s\n", read_text);
    }
    f_close(&f);
}

uint32_t FAT_GetFilesize(const char* filename) {
    FRESULT res;
    FIL f;

    if (!FAT_Init()) return 0;

    res = f_open(&f, filename, FA_READ);
    if (res) return 0;

    return f_size(&f);
}

unsigned int FAT_ReadFileToBuffer(const char* filename, uint32_t offset, uint32_t len, uint8_t* buffer) {
    FRESULT res;
    UINT br = 0;

    if (!FAT_Init()) return 0;

    // Try to open the provided file.
    // If filename is NULL, try to work with already opened file.
    if (filename) {
        f_close(&persistent_file);
        res = f_open(&persistent_file, filename, FA_READ);
        if (res) 
        {
            printf("open error %d\n", res);
            return 0;
        }
    }

    res = f_lseek(&persistent_file, offset);
    if (res) printf("lseek error %d\n", res);

    res = f_read(&persistent_file, buffer, len, &br);
    if (res) printf("read error %d\n", res);

    return br;
}