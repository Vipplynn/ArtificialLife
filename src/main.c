#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/draw.h"
int main(int argc, char* argv[]){
    SDL_Window *window;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL3 window",
        640,
        480,
        SDL_WINDOW_OPENGL
    );
    
    if (window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, NULL);
    while (!done) {
        SDL_Event event;
        bool color = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        bool clear =SDL_RenderClear(renderer);
        color = SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        bool line = SDL_RenderLine(renderer, 0, 0, 120, 120);
        drawCircle(renderer, 50, 50, 10);

 
        bool present = SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
        // logic, present frame, etc
        if (line == false){
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create line");    
        }
    }
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

