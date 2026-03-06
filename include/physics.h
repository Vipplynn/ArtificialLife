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

struct body stormer_verlet(float pos, float acc, float dt);

struct body velocity_verlet(float pos, float acc, float dt);
