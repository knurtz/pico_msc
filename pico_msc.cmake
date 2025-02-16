macro(enable_pico_msc project)
    add_subdirectory(pico_msc)
    pico_set_linker_script(${project} ${CMAKE_CURRENT_SOURCE_DIR}/pico_msc/linker/custom_linker_script.ld)
    add_custom_command(TARGET ${project}
        POST_BUILD
        # todo: change to correct build folder, add flash offset as user parameter
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/pico_msc/tools/modify_uf2.py ${CMAKE_CURRENT_SOURCE_DIR}/build/${project}.uf2 0x10020000
        VERBATIM
    )
endmacro()