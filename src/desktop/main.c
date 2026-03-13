#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../../include/creature.h"

#define POP_SIZE 4

int main(int argc, char* argv[]) {
    srand(time(NULL));

    SDL_Window *window;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("An SDL3 window", 640, 480, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // --- Population setup ---
    struct creature population[POP_SIZE];
    float start_x[POP_SIZE][MAX_NODES];
    float start_y[POP_SIZE][MAX_NODES];
    float start_center_x[POP_SIZE];
    float base_lengths[POP_SIZE][MAX_SPRINGS];

    for (int p = 0; p < POP_SIZE; p++) {
        create_worm_creature(&population[p]);

        start_center_x[p] = 0.0f;
        for (int i = 0; i < population[p].node_count; i++) {
            start_x[p][i] = population[p].node_arr[i].x;
            start_y[p][i] = population[p].node_arr[i].y;
            start_center_x[p] += population[p].node_arr[i].x;
        }
        start_center_x[p] /= population[p].node_count;

        for (int i = 0; i < population[p].spring_count; i++) {
            base_lengths[p][i] = population[p].spring_arr[i].L;
        }
    }

    struct genome best_dna = population[0].dna;
    float best_fitness = -10000.0f;
    float current_fitness[POP_SIZE];
    for (int p = 0; p < POP_SIZE; p++) current_fitness[p] = 0.0f;

    float floor_level = 450.0f;
    float camera_x = 0.0f;
    int physics_speed = 1;
    float simulation_time = 0.0f;
    int generation = 0;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) done = true;
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_UP && physics_speed < 20) physics_speed++;
                if (event.key.key == SDLK_DOWN && physics_speed > 1) physics_speed--;
            }
        }

        // --- Generation reset ---
        if (simulation_time > 20.0f) {
            // Find best across population
            int gen_winner = 0;
            for (int p = 1; p < POP_SIZE; p++){
                if (current_fitness[p] > current_fitness[gen_winner]) {
                    gen_winner = p;   
                }
            }

            // Only update global best_dna if gen_winner actually beat 
            // all time record
            if (current_fitness[gen_winner] > best_fitness){
                best_fitness = current_fitness[gen_winner];
                best_dna = population[gen_winner].dna;
            }

            // Reset all creatures with best DNA + mutation
            for (int p = 0; p < POP_SIZE; p++){
                for (int i = 0; i < population[p].node_count; i++) {
                    population[p].node_arr[i].x      = start_x[p][i];
                    population[p].node_arr[i].y      = start_y[p][i];
                    population[p].node_arr[i].prev_x = start_x[p][i];
                    population[p].node_arr[i].prev_y = start_y[p][i];
                    population[p].node_arr[i].acc_x  = 0;
                    population[p].node_arr[i].acc_y  = 0;
                }
                population[p].dna = best_dna;
                if (p > 0){
                    mutate_genome(&population[p].dna, population[p].spring_count);
                }
                current_fitness[p] = 0.0f;
            }

            simulation_time = 0.0f;
            generation++;
        }

        // --- Physics update ---
        for (int step = 0; step < physics_speed; step++) {
            for (int p = 0; p < POP_SIZE; p++) {
                struct creature *c = &population[p];

                for (int i = 0; i < c->node_count; i++)
                    c->node_arr[i].acc_y = 500.0f;

                for (int i = 0; i < c->node_count; i++)
                    verlet(&c->node_arr[i], 0.016f);

                for (int i = 0; i < c->spring_count; i++) {
                    c->spring_arr[i].L = base_lengths[p][i]
                        + sinf(simulation_time * c->dna.frequencies[i] + c->dna.phases[i])
                        * c->dna.amplitudes[i];
                }

                for (int i = 0; i < c->spring_count; i++)
                    spring(c->spring_arr[i]);

                centroid_repulsion(c);

                for (int i = 0; i < c->node_count; i++)
                    ground_friction(&c->node_arr[i], floor_level);

                // Update fitness
                float sum = 0.0f;
                for (int i = 0; i < c->node_count; i++)
                    sum += c->node_arr[i].x;
                current_fitness[p] = (sum / c->node_count) - start_center_x[p];
            }
            simulation_time += 0.016f;
        }

        // --- Camera: follow the leading creature ---
        int lead = 0;
        for (int p = 1; p < POP_SIZE; p++) {
            if (current_fitness[p] > current_fitness[lead]) lead = p;
        }
        float avg_x = 0.0f;
        for (int i = 0; i < population[lead].node_count; i++)
            avg_x += population[lead].node_arr[i].x;
        avg_x /= population[lead].node_count;
        camera_x = avg_x - 320.0f;

        // --- Rendering ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Floor
        SDL_RenderLine(renderer, 0, floor_level, 640, floor_level);

        // All time record
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        float record_screen_x = (start_center_x[0] + best_fitness) - camera_x;

        SDL_RenderLine(renderer, record_screen_x, 0, record_screen_x, floor_level);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);



        // Distance markers
        int spacing = 200;
        int start_marker = (int)floorf((camera_x - start_center_x[0]) / spacing);
        for (int i = start_marker; i < start_marker + 6; i++) {
            float m_pos = start_center_x[0] + (i * spacing);
            float screen_x = m_pos - camera_x;
            SDL_RenderLine(renderer, screen_x, floor_level, screen_x, floor_level - 50.0f);
            char marker_label[16];
            snprintf(marker_label, sizeof(marker_label), "%dm", i * 10);
            SDL_RenderDebugText(renderer, screen_x + 5.0f, floor_level - 45.0f, marker_label);
        }

        // Draw all creatures
        for (int p = 0; p < POP_SIZE; p++) {
            struct creature *c = &population[p];

            // Tint the best creature white, others grey
            if (p == lead)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);

            for (int i = 0; i < c->spring_count; i++) {
                SDL_RenderLine(renderer,
                    c->spring_arr[i].node_a->x - camera_x,
                    c->spring_arr[i].node_a->y,
                    c->spring_arr[i].node_b->x - camera_x,
                    c->spring_arr[i].node_b->y);
            }
            for (int i = 0; i < c->node_count; i++) {
                drawCircle(renderer,
                    (int)(c->node_arr[i].x - camera_x),
                    (int)c->node_arr[i].y,
                    (int)c->node_arr[i].radius);
            }
        }

        // HUD
        char gen_text[32], fit_text[32], best_fit[32], speed_text[16];
        snprintf(gen_text,   sizeof(gen_text),   "Generation: %d",       generation);
        snprintf(fit_text,   sizeof(fit_text),   "Fitness: %.2f",        current_fitness[lead]);
        snprintf(best_fit,   sizeof(best_fit),   "Best Fitness: %.2f",   best_fitness);
        snprintf(speed_text, sizeof(speed_text), "Speed: %dx",           physics_speed);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 10.0f, 10.0f, gen_text);
        SDL_RenderDebugText(renderer, 10.0f, 30.0f, fit_text);
        SDL_RenderDebugText(renderer, 10.0f, 50.0f, best_fit);
        SDL_RenderDebugText(renderer, 10.0f, 70.0f, speed_text);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
