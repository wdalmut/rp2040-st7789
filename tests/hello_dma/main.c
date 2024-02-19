#include <pico/stdlib.h>
#include <stdio.h>

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

const int lcd_width = 128;
const int lcd_height = 128;

void my_display_flush(void)
{
    printf("display data flush completed.\n");
}

int main()
{
    stdio_init_all();

    uint16_t buffer[lcd_width * lcd_height];
    
    sleep_ms(3000);

    // initialize the lcd
    st7789_init(&lcd_config, lcd_width, lcd_height);
    st7789_set_dma_irq_handler(DMA_IRQ_1, my_display_flush);

    st7789_fill(0x0000);

    while (1) {
        for (int i = 0; i < lcd_width * lcd_height; i++) {
            buffer[i] = 0x0000;
        }

        // make screen black
        st7789_set_cursor(0, 0);
        st7789_dma_write(buffer, lcd_width * lcd_height * sizeof(uint16_t));

        // wait 1 second
        sleep_ms(1000);
        printf("sleep 1s\n");

        for (int i = 0; i < lcd_width * lcd_height; i++) {
            buffer[i] = 0xFFFF;
        }

        // make screen white
        st7789_set_cursor(0, 0);
        st7789_dma_write(buffer, lcd_width * lcd_height * sizeof(uint16_t));

        // wait 1 second
        sleep_ms(1000);
        printf("sleep 1s (2)\n");
    }
}