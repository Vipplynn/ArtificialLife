#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdbool.h>

//#include "../include/physics.h"
#include "../include/creature.h"
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
/*
    struct body ball = {
        .x = 320.0f,
        .y = 100.0f,
        .prev_x = 320.0f,
        .prev_y = 100.0f,
        .acc_x = 0.0f,
        .acc_y = 0.0f,
        .radius = 15.0f
    };
*/

    struct creature creature = {};

    create_tri_creature(&creature);
    float floor_level = 450.0f;
    
    float camera_x = 0.0f;

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
            //ball.acc_y = 9.81f;
            //verlet(&ball, 0.016f);
            //ground_friction(&ball, floor_level);

            for (int i = 0; i < 3; i++){
                creature.node_arr[i].acc_y = 600.0f;
            }            
            for (int i = 0; i < 3; i++){
                verlet(&creature.node_arr[i], 0.016f);
            }            
            for (int i = 0; i < 3; i++){
                spring(creature.spring_arr[i]);
            }
            for (int i = 0; i < 3; i++){
                ground_friction(&creature.node_arr[i], floor_level);
            }

            // rendering
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            // floor line
            SDL_RenderLine(renderer, 0, floor_level, 640, floor_level);

            // draw ball
            //drawCircle(renderer, (int)(ball.x - camera_x), (int)ball.y, (int)ball.radius);
            for (int i = 0; i < 3; i++){
                SDL_RenderLine(renderer, creature.spring_arr[i].node_a->x, creature.spring_arr[i].node_a->y, creature.spring_arr[i].node_b->x, creature.spring_arr[i].node_b->y);
            }
            for (int i = 0; i < 3; i++){
                drawCircle(renderer, (int)(creature.node_arr[i].x), (int)creature.node_arr[i].y, (int)creature.node_arr[i].radius);
            }

            printf("Node 0 Y: %f\n", creature.node_arr[0].y);

            SDL_RenderPresent(renderer);

    }
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

