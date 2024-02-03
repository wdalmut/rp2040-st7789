#include "pico/stdlib.h"
#include "st7789.h"

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

const int lcd_width = 200;
const int lcd_height = 200;

int main()
{
    // initialize the lcd
    st7789_init(&lcd_config, lcd_width, lcd_height);

    while (1) {
        // make screen black
        st7789_fill(0x0000);

        // wait 1 second
        sleep_ms(1000);

        // make screen white
        st7789_fill(0xffff);

        // wait 1 second
        sleep_ms(1000);
    }
}