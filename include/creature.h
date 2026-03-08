#include "draw.h"
#include "physics.h"

#define MAX_NODES 10
#define MAX_SPRINGS 30
struct genome {
    float frequencies[MAX_SPRINGS];
    float amplitudes[MAX_SPRINGS];
    float phases[MAX_SPRINGS];
};
struct creature {
    struct body node_arr[MAX_NODES];
    struct spring spring_arr[MAX_SPRINGS];
    
    struct genome dna;

    int node_count;
    int spring_count;
};


void create_tri_creature(struct creature *creature);
void mutate_genome(struct genome *g, int spring_count);
void create_worm_creature(struct creature *creature);
