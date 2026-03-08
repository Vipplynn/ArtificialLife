#include "draw.h"
#include "physics.h"

#define MAX_NODES 10
#define MAX_SPRINGS 30
struct creature {
    struct body node_arr[MAX_NODES];
    struct spring spring_arr[MAX_SPRINGS];

    int node_count;
    int spring_count;
};

void create_tri_creature(struct creature *creature);
