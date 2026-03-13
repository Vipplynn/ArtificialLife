#pragma once
#ifndef BAREMETAL
#include <SDL3/SDL_render.h>
typedef SDL_Renderer Renderer;
#else
typedef void Renderer;
#endif

void drawCircle(Renderer *renderer, int center_x, int center_y, int radius);

void drawPixel(Renderer *renderer, int x, int y);
