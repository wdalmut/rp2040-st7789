/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "platform.h"
#include "constants.h"
#include "sound.h"
#include "input.h"

#include "pico/stdlib.h"
#include "pico/st7789.h"

#define KEY_LEFT 15
#define KEY_RIGHT 26
#define KEY_DOWN 19
#define KEY_UP 6

#define KEY_SPACE 21
#define KEY_LEFT_SHIFT 20
#define KEY_ENTER 13
#define KEY_ESCAPE 16

// lcd configuration
const struct st7789_config lcd_config = {
    .spi      = spi1,
    .gpio_din = 11,
    .gpio_clk = 10,
    .gpio_cs  = 8,
    .gpio_dc  = 25,
    .gpio_rst = 27,
    .gpio_bl  = 24,
};

#define WINDOW_WIDTH 132
#define WINDOW_HEIGHT 128

static uint16_t elapsed_time = 0;

static uint16_t frame[WINDOW_WIDTH*WINDOW_HEIGHT];

static bool timer_callback( repeating_timer_t *rt )
{
    elapsed_time += 1;

    return true;
}

static void keyboard_init(void)
{
    // Define the GPIO pins you want to read
    const uint pins[] = {
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_SPACE,
        KEY_LEFT_SHIFT,
        KEY_ENTER,
        KEY_ESCAPE}; 

    // Set the GPIO pins as inputs
    for (int i = 0; i < 8; ++i) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);  // Enable pull-up resistor for input pins
    }
}

bool is_key_down(uint key)
{
    bool state = gpio_get(key);

    return state ? false : true;
}

/**
 * @brief PLATFORM initialize user-defined functions.
 * 
 */
void platform_init(void)
{
    stdio_init_all();
    
    static repeating_timer_t timer;
    add_repeating_timer_ms( 1, &timer_callback, NULL, &timer );

    // initialize the keyboard
    keyboard_init();

    // initialize the lcd
    st7789_init(&lcd_config, WINDOW_WIDTH, WINDOW_HEIGHT);
    st7789_fill(0xFFFF);
}

/**
 * @brief PLATFORM start drawing a new frame.
 * 
 */
void platform_draw_start(void)
{
    st7789_set_cursor(0, 0);
}

/**
 * @brief PLATFORM stop drawing current frame.
 * 
 */
void platform_draw_stop(void)
{
    st7789_write(frame, WINDOW_WIDTH * WINDOW_HEIGHT * 2);
}

/**
 * @brief PLATFORM write pixel value to screen.
 * 
 * @param x     X coordinate
 * @param y     Y coordinate
 * @param color Pixel color
 */
void platform_draw_pixel(uint8_t x, uint8_t y, bool color)
{
    uint8_t xi = SCREEN_HEIGHT-y;
    uint8_t yi = x;

    uint16_t index = xi + (WINDOW_WIDTH * yi);
    frame[index] = color ? 0xFFFF : 0x0000;
}

/**
 * @brief PLATFORM play audio effect through speaker.
 * 
 */
void platform_audio_play(void)
{
    /* Add definition here */
}

/**
 * @brief PLATFORM read user controls and update button state.
 * 
 */
void platform_input_update(void)
{
    input_button = 0;

    if (is_key_down(KEY_UP))
        input_button |= UP;
    if (is_key_down(KEY_DOWN))
        input_button |= DOWN;
    if (is_key_down(KEY_LEFT))
        input_button |= LEFT;
    if (is_key_down(KEY_RIGHT))
        input_button |= RIGHT;
    if (is_key_down(KEY_SPACE))
        input_button |= FIRE;
    if (is_key_down(KEY_LEFT_SHIFT))
        input_button |= JUMP;
    if (is_key_down(KEY_ENTER))
        input_button |= HOME;
    if (is_key_down(KEY_ESCAPE))
        input_button |= EXIT;
}

/**
 * @brief PLATFORM get time in milliseconds from start of execution.
 * 
 * @return uint32_t Start time in milliseconds
 */
uint32_t platform_millis(void)
{
    return elapsed_time;
}

/**
 * @brief PLATFORM apply blocking delay in milliseconds.
 * 
 * @param ms Delay in milliseconds
 */
void platform_delay(uint32_t ms)
{
    sleep_ms(ms);
}
