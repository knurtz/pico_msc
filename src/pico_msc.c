#include "pico_msc.h"
#include "flash.h"

bool PicoMSC_Init(void) {
    Flash_Init();
    tusb_init();

    return true;
}

void PicoMSC_Task(void) {
    tud_task();
    Flash_WriteCycle(false);
}