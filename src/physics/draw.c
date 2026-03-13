#include "../../include/draw.h"

#ifndef BAREMETAL
#include <SDL3/SDL.h>
#endif

void drawPixel(Renderer* renderer, int x, int y){
#ifdef BAREMETAL
    // TODO: implement register level SPI/TFT pixel draw here
    (void)renderer;
#else
    // Desktop version using SDL
    SDL_RenderPoint((SDL_Renderer*)renderer, (float)x, (float)y);
#endif
}

void drawCircle(Renderer *renderer, int center_x, int center_y, int radius){
    int x = 0;
    int y = -radius;
    int p = -radius;
    int cx = center_x;
    int cy = center_y;

    while (x < -y){
        if (p > 0){
            y += 1;
            p += 2*(x+y)+1;
        }
        else{
            p += 2*x + 1;
        }
        drawPixel(renderer, cx + x, cy + y);
        drawPixel(renderer, cx - x, cy + y);
        drawPixel(renderer, cx + x, cy - y);
        drawPixel(renderer, cx - x, cy - y);
        drawPixel(renderer, cx + y, cy + x);
        drawPixel(renderer, cx + y, cy - x);
        drawPixel(renderer, cx - y, cy + x);
        drawPixel(renderer, cx - y, cy - x);

        x += 1;
    }
}


