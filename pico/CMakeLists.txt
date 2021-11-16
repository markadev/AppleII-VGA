cmake_minimum_required(VERSION 3.16)

# Pull in SDK (must be before project)
include(cmake/pico_sdk_import.cmake)
include(cmake/gitversion.cmake)

project(applevga C)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()


add_executable(applevga)

pico_generate_pio_header(applevga
    ${CMAKE_CURRENT_SOURCE_DIR}/vga.pio)

target_sources(applevga PUBLIC
    main.c
    render.c
    vga.c
)
target_include_directories(applevga PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(applevga PUBLIC
    pico_multicore
    pico_stdlib
    hardware_dma
    hardware_pio
)

pico_add_extra_outputs(applevga)
