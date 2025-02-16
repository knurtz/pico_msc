add_library(pico_msc INTERFACE)

target_include_directories(pico_msc INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/..            # for user defined config file
    src
    src/fatfs/source
)

include(${CMAKE_CURRENT_LIST_DIR}/ffconf.cmake)

# Add all source code files
target_sources(pico_msc INTERFACE
    # Mass storage    
    ${CMAKE_CURRENT_LIST_DIR}/src/msc_disk.c
    ${CMAKE_CURRENT_LIST_DIR}/src/flash_functions.c
    
    # FatFS
    ${CMAKE_CURRENT_LIST_DIR}/src/fat.c
    ${CMAKE_CURRENT_LIST_DIR}/src/fatfs/source/ff.c
    ${CMAKE_CURRENT_LIST_DIR}/src/fatfs/source/ffunicode.c
    ${CMAKE_CURRENT_LIST_DIR}/src/diskio_picomsc.c

    # USB
    ${CMAKE_CURRENT_LIST_DIR}/src/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/src/usb_callbacks.c
)

# Import pico_stdlib which imports commonly used features
target_link_libraries(pico_msc INTERFACE
    pico_stdlib
    pico_flash
    hardware_flash
    hardware_sync
    tinyusb_device
)