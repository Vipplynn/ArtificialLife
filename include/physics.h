struct creature;

struct body {
    float x, y;
    float prev_x, prev_y;
    float acc_x, acc_y;
    float radius;
};

struct spring {
    struct body *node_a;
    struct body *node_b;

    float L;
};

void verlet(struct body *body, float dt);

float fast_sqrt(float number);

void spring(struct spring s);

void ground_friction(struct body *node, float floor_height);

void centroid_repulsion(struct creature *creature);
