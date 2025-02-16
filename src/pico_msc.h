#ifndef PICO_MSC_H
#define PICO_MSC_H

#include "pico/stdlib.h"

/**
 * @brief Initialize mass storage device and USB connection
 * 
 * @return true if PicoMSC is ready to use
 * @return false if any errors occured and it is not safe to use PicoMSC
 * 
 * @note Call this method once during setup of your code
 */
bool PicoMSC_Init(void);

/**
 * @brief USB routine and periodic flash write cycles
 * 
 * @note Make sure this method gets called in a tight main loop
 */
void PicoMSC_Task(void);

#endif  // PICO_MSC_H