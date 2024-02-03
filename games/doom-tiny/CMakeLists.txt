set(DOOM_PICO ${CMAKE_CURRENT_SOURCE_DIR}/doom-pico)

add_executable(doom-pico
	${DOOM_PICO}/src/display.c
    ${DOOM_PICO}/src/entities.c
    ${DOOM_PICO}/src/game.c
    ${DOOM_PICO}/src/input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform.c
    ${DOOM_PICO}/src/sound.c
    ${DOOM_PICO}/src/utils.c
)

target_include_directories(doom-pico PRIVATE
    ${DOOM_PICO}/inc
)

target_compile_definitions(doom-pico
    PRIVATE USE_RAYLIB=0
    )

target_compile_options(doom-pico PRIVATE -Wall)

target_link_libraries(doom-pico
	pico_stdlib
	pico_util
	libst7789
)

pico_enable_stdio_usb(doom-pico 1) # enable usb output
pico_enable_stdio_uart(doom-pico 0) # disable uart output

# create map/bin/hex file etc.
pico_add_extra_outputs(doom-pico)