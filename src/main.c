#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/draw.h"
#include "../include/physics.h"
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

    struct body ball = {
        .x = 320.0f,
        .y = 100.0f,
        .prev_x = 320.0f,
        .prev_y = 100.0f,
        .acc_x = 0.0f,
        .acc_y = 0.0f,
        .radius = 15.0f
    };

    float floor_level = 450.0f;


    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, NULL);
    while (!done) {
        SDL_Event event;


 
        bool present = SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
        // logic, present frame, etc
        ball.acc_y = 500.0f;
            verlet(&ball, 0.016f);
            ground_friction(&ball, floor_level);

            // rendering
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            // floor line
            SDL_RenderLine(renderer, 0, floor_level, 640, floor_level);

            // draw ball
            drawCircle(renderer, (int)ball.x, (int)ball.y, (int)ball.radius);

            SDL_RenderPresent(renderer);

    }
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

