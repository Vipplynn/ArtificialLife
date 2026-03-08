#include "../include/creature.h"
#include <stdlib.h>

void create_tri_creature(struct creature *creature){
   creature->node_count = 3;
   creature->spring_count = 3;

   // node init

   creature->node_arr[0].x = 300;
   creature->node_arr[0].y = 100;
   creature->node_arr[0].prev_x = 300;
   creature->node_arr[0].prev_y = 100;

   creature->node_arr[1].x = 340;
   creature->node_arr[1].y = 100;
   creature->node_arr[1].prev_x = 340;
   creature->node_arr[1].prev_y = 100;   


   
   creature->node_arr[2].x = 320;
   creature->node_arr[2].y = 70;   
   creature->node_arr[2].prev_x = 320;
   creature->node_arr[2].prev_y = 70;

   for (int i = 0; i < 3; i++){
        creature->node_arr[i].acc_x = 0.0f;
        creature->node_arr[i].acc_y = 0.0f;
        creature->node_arr[i].radius = 10.0f;

   }

   // spring connection
   creature->spring_arr[0].node_a = &creature->node_arr[0];
   creature->spring_arr[0].node_b = &creature->node_arr[1];

   creature->spring_arr[1].node_a = &creature->node_arr[0];
   creature->spring_arr[1].node_b = &creature->node_arr[2];

   creature->spring_arr[2].node_a = &creature->node_arr[1];
   creature->spring_arr[2].node_b = &creature->node_arr[2];

   // spring rest length calculation
   for (int i = 0; i < 3; i++){
       float dx = creature->spring_arr[i].node_b->x - creature->spring_arr[i].node_a->x;

       float dy = creature->spring_arr[i].node_b->y - creature->spring_arr[i].node_a->y;

       float dist_sq = (dx * dx) + (dy * dy);

       float inv_dist = fast_sqrt(dist_sq);

       creature->spring_arr[i].L = 1.0f / inv_dist;
   }


   // DNA initialization
   
   for (int i = 0; i < creature->spring_count; i++){
        creature->dna.frequencies[i] = 2.0f + ((rand() % 100) / 100.0f) * 8.0f;
        creature->dna.amplitudes[i] = 5.0f + ((rand() % 100) / 100.0f) * 15.0f;
        creature->dna.phases[i] = ((rand() % 100 / 100.0f) * (2.0f * 3.14159f));
   }
}


void mutate_genome(struct genome *g, int spring_count){
    for (int i = 0; i < spring_count; i++){
        // 20% for mutation
        if ((rand() % 100) < 20){
            g->frequencies[i] += ((rand() % 100) / 50.0f - 1.0f) * 0.5f;

            g->amplitudes[i] += ((rand() % 100) / 50.0f - 1.0f) * 2.0f;

            g->phases[i] += ((rand() % 100) / 50.0f - 1.0f) * 0.5f;

        }
    }
}

/* worm
o-------o-------o
|  \  / |  \  / |
|   \/  |   \/  |
|   /\  |   /\  |
|  /  \ |  /  \ |
o-------o-------o*/
void create_worm_creature(struct creature *creature){
    creature->node_count = 10;
    creature->spring_count = 21;

    float start_x = 300.0f;
    float start_y = 400.0f;
    float seg_w = 40.0f;
    float seg_h = 30.0f;

    // top row: 0,1,2,3,4
    for (int i = 0; i < 5; i++){
        creature->node_arr[i].x = start_x + i * seg_w;
        creature->node_arr[i].y = start_y;
        creature->node_arr[i].prev_x = start_x + i * seg_w;
        creature->node_arr[i].prev_y = start_y;
    }
    // bottom row: 5,6,7,8,9
    for (int i = 0; i < 5; i++){
        creature->node_arr[i+5].x = start_x + i * seg_w;
        creature->node_arr[i+5].y = start_y + seg_h;
        creature->node_arr[i+5].prev_x = start_x + i * seg_w;
        creature->node_arr[i+5].prev_y = start_y + seg_h;
    }
    for (int i = 0; i < creature->node_count; i++){
        creature->node_arr[i].acc_x  = 0.0f;
        creature->node_arr[i].acc_y  = 0.0f;
        creature->node_arr[i].radius = 10.0f;
    }

    int s = 0;

    // top horizontals (4): 0-1, 1-2, 2-3, 3-4
    for (int i = 0; i < 4; i++){
        creature->spring_arr[s].node_a = &creature->node_arr[i];
        creature->spring_arr[s].node_b = &creature->node_arr[i+1];
        s++;
    }
    // bottom horizontals (4): 5-6, 6-7, 7-8, 8-9
    for (int i = 0; i < 4; i++){
        creature->spring_arr[s].node_a = &creature->node_arr[i+5];
        creature->spring_arr[s].node_b = &creature->node_arr[i+6];
        s++;
    }
    // verticals (5): 0-5, 1-6, 2-7, 3-8, 4-9
    for (int i = 0; i < 5; i++){
        creature->spring_arr[s].node_a = &creature->node_arr[i];
        creature->spring_arr[s].node_b = &creature->node_arr[i+5];
        s++;
    }
    // diagonals (8): 2 per segment
    for (int i = 0; i < 4; i++){
        creature->spring_arr[s].node_a = &creature->node_arr[i];    // top-left to bottom-right
        creature->spring_arr[s].node_b = &creature->node_arr[i+6];
        s++;
        creature->spring_arr[s].node_a = &creature->node_arr[i+1];  // top-right to bottom-left
        creature->spring_arr[s].node_b = &creature->node_arr[i+5];
        s++;
    }

    // rest lengths
    for (int i = 0; i < creature->spring_count; i++){
        float dx = creature->spring_arr[i].node_b->x - creature->spring_arr[i].node_a->x;
        float dy = creature->spring_arr[i].node_b->y - creature->spring_arr[i].node_a->y;
        float dist_sq = (dx * dx) + (dy * dy);
        creature->spring_arr[i].L = 1.0f / fast_sqrt(dist_sq);
    }

    // DNA
    for (int i = 0; i < 13; i++){
        creature->dna.frequencies[i] = 2.0f + ((rand() % 100) / 100.0f) * 8.0f;
        creature->dna.amplitudes[i] = 5.0f + ((rand() % 100) / 100.0f) * 15.0f;
        creature->dna.phases[i] = ((rand() % 100) / 100.0f) * (2.0f * 3.14159f);
    }
    for (int i = 13; i < creature->spring_count; i++){
        creature->dna.frequencies[i] = 0.0f;
        creature->dna.amplitudes[i] = 0.0f;
        creature->dna.phases[i] = 0.0f;
    }
}
