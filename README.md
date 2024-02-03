# RP2040 Doom

try to port the game Doom to the RP2040 using the Waveshare
rp2040 pi zero and the waveshare 128x128 st7789 LCD display.

## Hardware

 * https://www.waveshare.com/wiki/RP2040-PiZero
 * https://www.waveshare.com/wiki/1.44inch_LCD_HAT

## Tiny Doom

The first example is based on Doom Pico (a port of Doom
Nano) from [Gualor](https://github.com/Gualor/doom-pico).

![doom](./games/doom-tiny/doom.jpeg)

## Notes

> The ST7735S is a 132*162 pixel LCD controller, but the pixel of the 1.44-inch
  LCD HAT is 128*128. So we have made some processing on the display: the horizontal
  direction starts from the second pixel to guarantee the location of RAM in the
  LCD is consistent with the actual location at the same time.

> The Joystick left is not GPIO5 but GPIO15
