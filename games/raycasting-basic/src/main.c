#include <stdio.h>
#include <pico/stdlib.h>
#include <math.h>

#include "platform.h"
#include "input.h"

#define mapWidth 24
#define mapHeight 24
#define screenWidth WINDOW_WIDTH
#define screenHeight WINDOW_HEIGHT

int worldMap[mapWidth][mapHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int main(void)
{
    platform_init();

    double posX = 22, posY = 12;      // x and y start position
    double dirX = -1, dirY = 0;       // initial direction vector
    double planeX = 0, planeY = 0.66; // the 2d raycaster version of camera plane

    double time = 0;    // time of current frame
    double oldTime = 0; // time of previous frame

    while (1) {
        platform_draw_start();

        for (int x = 0; x < screenWidth; x++)
        {
            // calculate ray position and direction
            double cameraX = 2 * x / (double)screenWidth - 1; // x-coordinate in camera space
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;
            // which box of the map we're in
            int mapX = (int)posX;
            int mapY = (int)posY;

            // length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            // length of ray from one x or y-side to next x or y-side
            // these are derived as:
            // deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
            // deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
            // which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
            // where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
            // unlike (dirX, dirY) is not 1, however this does not matter, only the
            // ratio between deltaDistX and deltaDistY matters, due to the way the DDA
            // stepping further below works. So the values can be computed as below.
            //  Division through zero is prevented, even though technically that's not
            //  needed in C++ with IEEE 754 floating point values.
            double deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
            double deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);

            double perpWallDist;

            // what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            int hit = 0; // was there a wall hit?
            int side = 0;    // was a NS or a EW wall hit?
            // calculate step and initial sideDist
            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }
            // perform DDA
            while (hit == 0)
            {
                // jump to next map square, either in x-direction, or in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                // Check if ray has hit a wall
                if (worldMap[mapX][mapY] > 0)
                    hit = 1;
            }
            // Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
            // hit to the camera plane. Euclidean to center camera point would give fisheye effect!
            // This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
            // for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
            // because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
            // steps, but we subtract deltaDist once because one step more into the wall was taken above.
            if (side == 0)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);

            // Calculate height of line to draw on screen
            int lineHeight = (int)(screenHeight / perpWallDist);

            // calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight)
                drawEnd = screenHeight - 1;

            // choose wall color
            uint16_t color;
            switch (worldMap[mapX][mapY])
            {
            case 1:
                color = 0xf800;
                break; // red
            case 2:
                color = 0x07e0;
                break; // green
            case 3:
                color = 0x081f;
                break; // blue
            case 4:
                color = 0x9caf;
                break; // ocra
            default:
                color = 0xff40;
                break; // yellow
            }

            // give x and y sides different brightness
            if (side == 1)
            {
                color = color / 2;
            }

            for (uint16_t y = drawStart; y < drawEnd; y++) {
                platform_draw_pixel(x, y, color);  
            }
        }

        platform_draw_stop();

        oldTime = time;
        do {
            // timing for input and FPS counter
            time = platform_millis();
        } while(time - oldTime < 30); // about 30 fps
        
        double frameTime = (time - oldTime) / 1000.0;
        double moveSpeed = frameTime * 5.0; // the constant value is in squares/second
        double rotSpeed = frameTime * 3.0;  // the constant value is in radians/second

        uint32_t key_pressed = platform_input_update();
        if (key_pressed == UP)
        {
            if(worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == false) posX += dirX * moveSpeed;
            if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
        }
        //move backwards if no wall behind you
        if (key_pressed == DOWN)
        {
            if(worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == false) posX -= dirX * moveSpeed;
            if(worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
        }
        // rotate to the right
        if (key_pressed == RIGHT)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }
        // rotate to the left
        if (key_pressed == LEFT)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
    }
}