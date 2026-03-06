#include "../include/physics.h"

#define FRICTION_COEFFICIENT 0.9f
#define BOUNCE_FACTOR 0.3f

void verlet(struct body *b, float dt){
    float temp_x = b->x;
    float temp_y = b->y;

    // calculate new pos
    b->x = 2.0*b->x - b->prev_x + b->acc_x * (dt * dt);
    b->y = 2.0*b->y - b->prev_y + b->acc_y * (dt * dt);


    b->prev_x = temp_x;
    b->prev_y = temp_y;

    b->acc_x = 0;
    b->acc_y = 0;
}

float fast_sqrt(float number){
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = y * (threehalfs - (x2 * y * y));

    return y;
}

void spring(struct spring s){
   // find distance
   float dis_x = s.node_b->x - s.node_a->x;
   float dis_y = s.node_b->y - s.node_a->y;
   float inv_distance = fast_sqrt((dis_x * dis_x) + (dis_y * dis_y));
   float distance = 1.0f / inv_distance;
   // calculate error
   float error_ratio = (s.L - distance)*inv_distance * 0.5f; 

   // calculate move amount
   float offset_x = dis_x * error_ratio;
   float offset_y = dis_y * error_ratio;

   // update nodes
   s.node_a->x -= offset_x;
   s.node_a->y -= offset_y;
   s.node_b->x += offset_x;
   s.node_b->y += offset_y;
}

void ground_friction(struct body *node, float floor_height){
    if (node->y + node->radius >= floor_height){ // opposite because top left of window is (0,0) and goes down
        node->y = floor_height - node->radius;

        float x_prev_new = node->x + (node->prev_x - node->x) * FRICTION_COEFFICIENT;
    
        node->prev_x = x_prev_new;

        // bounce
        float y_prev_new = node->y + (node->prev_y - node->y) * (-1 * BOUNCE_FACTOR);

        node->prev_y = y_prev_new;
    }
}
