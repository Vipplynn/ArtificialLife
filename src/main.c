#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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

    struct genome best_dna = {};

    create_tri_creature(&creature);
    float floor_level = 450.0f;
    
    float camera_x = 0.0f;
    float simulation_time = 0.0f;
    float base_L = creature.spring_arr[0].L;
    
    float base_node_x = creature.node_arr[0].x;
    float best_fitness = -10000.0f; 

    int generation = 0;

    float base_lengths[creature.spring_count];
    for (int i = 0; i < creature.spring_count; i++){
        base_lengths[i] = creature.spring_arr[i].L;
    }

    float start_x[3], start_y[3];
    for (int i = 0; i < 3; i++){
        start_x[i] = creature.node_arr[i].x;
        start_y[i] = creature.node_arr[i].y;
    }


    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);
    while (!done) {
        SDL_Event event;


 
        //bool present = SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
        // logic, present frame, etc
            //ball.acc_y = 9.81f;
            //verlet(&ball, 0.016f);
            //ground_friction(&ball, floor_level);
        if (simulation_time > 10.0f) {
            float current_fitness = creature.node_arr[0].x - start_x[0];
            
            if (current_fitness > best_fitness){
                best_fitness = current_fitness;

                best_dna = creature.dna;
            }

            for (int i = 0; i < 3; i++){
                creature.node_arr[i].x = start_x[i];
                creature.node_arr[i].y = start_y[i];
                creature.node_arr[i].prev_x = start_x[i];
                creature.node_arr[i].prev_y = start_y[i];
                creature.node_arr[i].acc_x = 0;
                creature.node_arr[i].acc_y = 0;
            }
            simulation_time = 0.0f;
            generation++;

            creature.dna = best_dna;
            mutate_genome(&creature.dna);
        }
            float avg_x = (creature.node_arr[0].x + creature.node_arr[1].x + creature.node_arr[2].x) / 3.0f;

            camera_x = avg_x - 320.f;

            for (int i = 0; i < 3; i++){
                creature.node_arr[i].acc_y = 981.0f;
            }            
            for (int i = 0; i < 3; i++){
                verlet(&creature.node_arr[i], 0.016f);
            }

            for (int i = 0; i < creature.spring_count; i++){
                float freq = creature.dna.frequencies[i];
                float amp = creature.dna.amplitudes[i];
                float phase = creature.dna.phases[i];

                creature.spring_arr[i].L = base_lengths[i] + sinf(simulation_time * freq + phase) * amp;
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
                SDL_RenderLine(renderer, creature.spring_arr[i].node_a->x - camera_x, creature.spring_arr[i].node_a->y, creature.spring_arr[i].node_b->x - camera_x, creature.spring_arr[i].node_b->y);
            }
            for (int i = 0; i < 3; i++){
                drawCircle(renderer, (int)(creature.node_arr[i].x - camera_x), (int)creature.node_arr[i].y, (int)creature.node_arr[i].radius);
            }

            printf("Node 0 distance travelled: %f\n", creature.node_arr[0].x - start_x[0]);
            char gen_text[32], fit_text[32], best_fit[32];
            float displacement = creature.node_arr[0].x - start_x[0];
            snprintf(gen_text, sizeof(gen_text), "Generation: %d", generation);
            snprintf(fit_text, sizeof(fit_text), "Fitness: %.2f", displacement);
            snprintf(best_fit, sizeof(best_fit), "Best Fitness: %.2f", best_fitness);

            SDL_RenderDebugText(renderer, 10.0f, 10.0f, gen_text);
            SDL_RenderDebugText(renderer, 10.0f, 30.0f, fit_text);
            SDL_RenderDebugText(renderer, 10.0f, 50.0f, best_fit);

            simulation_time += 0.016f;

            SDL_RenderPresent(renderer);

    }
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

