#include <math.h>
#include <stdlib.h>

#define GRID_SIZE 32
#define PARTICLE_COUNT 1000
#define DT 0.01f
#define GRAVITY_MAGNITUDE -9.8f

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    Vec2 pos;
    Vec2 vel;
} Particle;

typedef struct {
    Vec2 velocity;
    float mass;
} GridCell;

Particle particles[PARTICLE_COUNT];
GridCell grid[GRID_SIZE][GRID_SIZE];
Vec2 gravity = { 0.0f, -9.8f };

void init_particles();
void clear_grid();
void particle_to_grid();
void apply_gravity();
void set_gravity_direction(Vec2 dir);
void grid_to_particle();
void advect_particles();
void apply_repulsion();
void step_simulation();


