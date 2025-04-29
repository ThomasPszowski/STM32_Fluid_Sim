#include "SimC.h"

void init_particles() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        particles[i].pos.x = 10.0f + (float)rand() / RAND_MAX * GRID_SIZE;
        particles[i].pos.y = 20.0f + (float)rand() / RAND_MAX * GRID_SIZE;
        particles[i].vel.x = 0.0f;
        particles[i].vel.y = 0.0f;
    }
}

void clear_grid() {
    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++) {
            grid[y][x].velocity.x = 0.0f;
            grid[y][x].velocity.y = 0.0f;
            grid[y][x].mass = 0.0f;
        }
}

void particle_to_grid() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        Particle* p = &particles[i];
        int x = (int)p->pos.x;
        int y = (int)p->pos.y;

        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
            grid[y][x].velocity.x += p->vel.x;
            grid[y][x].velocity.y += p->vel.y;
            grid[y][x].mass += 1.0f;
        }
    }

    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x].mass > 0.0f) {
                grid[y][x].velocity.x /= grid[y][x].mass;
                grid[y][x].velocity.y /= grid[y][x].mass;
            }
        }
}


void apply_gravity() {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x].mass > 0.0f) {
                grid[y][x].velocity.x += gravity.x * DT;
                grid[y][x].velocity.y += gravity.y * DT;
            }
        }
    }
}

void set_gravity_direction(Vec2 dir) {
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 1e-6f) {
        gravity.x = dir.x / len * GRAVITY_MAGNITUDE;
        gravity.y = dir.y / len * GRAVITY_MAGNITUDE;
    }
    else {
        gravity.x = 0.0f;
        gravity.y = 0.0f;
    }
}

void grid_to_particle() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        Particle* p = &particles[i];
        int x = (int)p->pos.x;
        int y = (int)p->pos.y;

        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
            Vec2 gridVel = grid[y][x].velocity;
            p->vel = gridVel;
        }
    }
}

void advect_particles() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        Particle* p = &particles[i];

        p->pos.x += p->vel.x * DT;
        p->pos.y += p->vel.y * DT;

        if (p->pos.x < 0) { p->pos.x = 0; p->vel.x *= -0.5f; }
        if (p->pos.x >= GRID_SIZE) { p->pos.x = GRID_SIZE - 0.1; p->vel.x *= -0.5f; }

        if (p->pos.y < 0) { p->pos.y = 0; p->vel.y *= -0.5f; }
        if (p->pos.y >= GRID_SIZE) { p->pos.y = GRID_SIZE - 0.1; p->vel.y *= -0.5f; }
    }
}

void apply_repulsion() {
    const float minDist = 2.0f;
    const float k = 10.0f;
    const float eps = 0.001f;

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        for (int j = i + 1; j < PARTICLE_COUNT; j++) {
            Vec2 dp = {
                particles[i].pos.x - particles[j].pos.x,
                particles[i].pos.y - particles[j].pos.y
            };
            float dist2 = dp.x * dp.x + dp.y * dp.y;
            if (dist2 < eps) {
                particles[i].vel.x *= -1;
                particles[i].vel.y *= -1;
            }
            if (dist2 < minDist * minDist) {
                float dist = sqrtf(dist2);
                float force = k * (minDist - dist) / dist;

                Vec2 dir = { dp.x / dist, dp.y / dist };
                particles[i].vel.x += dir.x * force * DT;
                particles[i].vel.y += dir.y * force * DT;
                particles[j].vel.x -= dir.x * force * DT;
                particles[j].vel.y -= dir.y * force * DT;
            }

        }
    }
}


void step_simulation() {
    clear_grid();
    particle_to_grid();
    apply_gravity();
    grid_to_particle();
    apply_repulsion();
    advect_particles();
}


