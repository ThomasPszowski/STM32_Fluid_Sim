#pragma once
#include <math.h>
#include <stdlib.h>

#define GRID_SIZE 8
#define REPULSION_GRID_DIM 8
#define REPULSION_GRID_SIZE (REPULSION_GRID_DIM * REPULSION_GRID_DIM)
#define PARTICLE_COUNT 30
#define DT 0.1f
#define GRAVITY_MAGNITUDE -1.0f
#define OUT_BUFFER_DIM 16
#define out_string_len (OUT_BUFFER_DIM * OUT_BUFFER_DIM * 2 + OUT_BUFFER_DIM * 2 + 9)

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

extern Particle particles[PARTICLE_COUNT];
extern GridCell grid[GRID_SIZE][GRID_SIZE];
extern Particle* particle_lookup[PARTICLE_COUNT + 1];
extern int indexTable[REPULSION_GRID_DIM][REPULSION_GRID_DIM];
extern Vec2 gravity;
extern char out_buffer[OUT_BUFFER_DIM][OUT_BUFFER_DIM];
extern char out_string[out_string_len];

char* get_out_string();
void generate_output();
void generate_output_external(int **out_buffer, int out_buffer_dim);
void init_particles();
void init_particles_middle();
void clear_grid();
void particle_to_grid();
void apply_gravity();
void set_gravity_direction(Vec2 dir);
void grid_to_particle();
void advect_particles();
void apply_repulsion();
void apply_repulsion(Particle* p1, Particle* p2);
void apply_repulsion_optimized_without_hash();
void advect_particles_circular();
void step_simulation();


