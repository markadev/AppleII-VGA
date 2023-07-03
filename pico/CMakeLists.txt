cmake_minimum_required(VERSION 3.16)

# Pull in SDK (must be before project)
include(cmake/pico_sdk_import.cmake)

project(applevga)
set(CMAKE_C_STANDARD 11)

set(APPLE_MODEL "IIPLUS" CACHE STRING "The model of Apple II to support")
message("Target Apple model is ${APPLE_MODEL}")
add_definitions(-DAPPLE_MODEL_${APPLE_MODEL}=1)

pico_sdk_init()

add_compile_options(-Wall -Wextra -Wnull-dereference -Wno-unused-parameter -Wno-unused-function)


add_executable(applevga)

pico_generate_pio_header(applevga
    ${CMAKE_CURRENT_SOURCE_DIR}/vga.pio)
pico_generate_pio_header(applevga
    ${CMAKE_CURRENT_SOURCE_DIR}/abus.pio)

target_sources(applevga PUBLIC
    main.c
    abus.c
    buffers.c
    colors.c
    render.c
    render_hires.c
    render_lores.c
    render_testpat.c
    render_text.c
    textfont_iie.c
    textfont_iiplus.c
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
