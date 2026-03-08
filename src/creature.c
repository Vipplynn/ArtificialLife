#include "../include/creature.h"

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
}
