add_executable(hello_keyboard
	main.c
)

target_compile_options(hello_keyboard PRIVATE -Wall)

target_link_libraries(hello_keyboard
	pico_stdlib
	pico_util
	libst7789
)

pico_enable_stdio_usb(hello_keyboard 1) # enable usb output
pico_enable_stdio_uart(hello_keyboard 0) # disable uart output

# create map/bin/hex file etc.
pico_add_extra_outputs(hello_keyboard)