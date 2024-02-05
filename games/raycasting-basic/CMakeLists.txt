add_executable(raycasting-basic
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

target_include_directories(raycasting-basic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_compile_options(raycasting-basic PRIVATE -Wall)

target_link_libraries(raycasting-basic
	pico_stdlib
	pico_util
	libst7789
)

pico_enable_stdio_usb(raycasting-basic 1) # enable usb output
pico_enable_stdio_uart(raycasting-basic 0) # disable uart output

# create map/bin/hex file etc.
pico_add_extra_outputs(raycasting-basic)