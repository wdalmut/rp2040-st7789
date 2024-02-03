#include <pico/stdlib.h>
#include <stdio.h>

int main()
{
    stdio_init_all();

    sleep_ms(1000);

    // Define the GPIO pins you want to read
    const uint pins[] = {15, 21, 20, 16, 6, 19, 26, 13};  // Change these to the desired GPIO pin numbers

    // Set the GPIO pins as inputs
    for (int i = 0; i < 8; ++i) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);  // Enable pull-up resistor for input pins
    }

    while (1) {
        // Read and print the state of each GPIO pin
        for (int i = 0; i < 8; ++i) {
            bool state = gpio_get(pins[i]);
            printf("GPIO Pin %d state: %s\n", pins[i], state ? "HIGH" : "LOW");
        }

        printf("\n\n");

        // Add a delay to avoid continuous printout
        sleep_ms(1000);
    }

    return 0;
}