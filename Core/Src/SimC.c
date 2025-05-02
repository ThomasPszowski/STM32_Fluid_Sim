#include "SimC.h"

Particle particles[PARTICLE_COUNT];
GridCell grid[GRID_SIZE][GRID_SIZE];
Vec2 gravity = { 0.0f, -1.0f };
int indexTable[REPULSION_GRID_DIM][REPULSION_GRID_DIM];
Particle* particle_lookup[PARTICLE_COUNT + 1];
char out_buffer[OUT_BUFFER_DIM][OUT_BUFFER_DIM] = { 0 };

char out_string[out_string_len] = { 0 };

const float minDist = (float)GRID_SIZE / REPULSION_GRID_DIM + 0.001;
const float k = 1.5f;
const float eps = 0.05f;

char* get_out_string() {
	int index = 0;

	out_string[index++] = '\033';
	out_string[index++] = '[';
	out_string[index++] = '2';
	out_string[index++] = 'J';
	out_string[index++] = '\033';
	out_string[index++] = '[';
	out_string[index++] = 'H';

	/*for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			if (grid[y][x].mass > 0.0f) {
				out_string[index++] = '#';
			}
			else {
				out_string[index++] = '.';
			}
		}
		out_string[index++] = '\r';
		out_string[index++] = '\n';
	}*/
	for (int y = 0; y < OUT_BUFFER_DIM; y++) {
		for (int x = 0; x < OUT_BUFFER_DIM; x++) {
			out_buffer[y][x] = ' ';
		}
	}
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		Particle* p = particles + i;
		int x = p->pos.x / GRID_SIZE * OUT_BUFFER_DIM;
		int y = p->pos.y / GRID_SIZE * OUT_BUFFER_DIM;
		out_buffer[y][x] = '#';
	}
	for (int y = 0; y < OUT_BUFFER_DIM; y++) {
		for (int x = 0; x < OUT_BUFFER_DIM; x++) {
			out_string[index++] = out_buffer[y][x];
			out_string[index++] = out_buffer[y][x];
		}
		out_string[index++] = '\r';
		out_string[index++] = '\n';
	}
	out_string[index] = '\0';

	return out_string;
}

void generate_output(){
	for (int y = 0; y < OUT_BUFFER_DIM; y++) {
		for (int x = 0; x < OUT_BUFFER_DIM; x++) {
			out_buffer[y][x] = 0;
		}
	}
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		Particle* p = particles + i;
		int x = p->pos.x / GRID_SIZE * OUT_BUFFER_DIM;
		int y = p->pos.y / GRID_SIZE * OUT_BUFFER_DIM;
		out_buffer[y][x] = 1;
	}
}

void generate_output_external(int **out_buffer, int out_buffer_dim) {
	for (int y = 0; y < out_buffer_dim; y++) {
		for (int x = 0; x < out_buffer_dim; x++) {
			out_buffer[y][x] = 0;
		}
	}
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		Particle* p = particles + i;
		int x = p->pos.x / GRID_SIZE * out_buffer_dim;
		int y = p->pos.y / GRID_SIZE * out_buffer_dim;
		out_buffer[y][x] = 1;
	}
}

void init_particles() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        particles[i].pos.x = (float)rand() / RAND_MAX * GRID_SIZE;
        particles[i].pos.y = (float)rand() / RAND_MAX * GRID_SIZE;
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

void apply_repulsion_global() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        for (int j = i + 1; j < PARTICLE_COUNT; j++) {
            apply_repulsion(particles + i, particles + j);
        }
    }
}

void advect_particles_circular() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        Particle* p = &particles[i];

        p->pos.x += p->vel.x * DT;
        p->pos.y += p->vel.y * DT;

#define CENTER (GRID_SIZE / 2.0f)
#define SQUARED_RADIUS (CENTER * CENTER)

        float dx = p->pos.x - CENTER;
        float dy = p->pos.y - CENTER;
        float dist2 = dx * dx + dy * dy;

        if (dist2 > SQUARED_RADIUS) {
            float dist = sqrtf(dist2);
            float nx = dx / dist;  // normal x
            float ny = dy / dist;  // normal y


            p->pos.x = CENTER + nx * CENTER;
            p->pos.y = CENTER + ny * CENTER;


            float dot = p->vel.x * nx + p->vel.y * ny;
            p->vel.x -= 2.0f * dot * nx;
            p->vel.y -= 2.0f * dot * ny;

        }
    }
}

void apply_repulsion(Particle* p1, Particle* p2) {
    Vec2 dp = {
        p1->pos.x - p2->pos.x,
        p1->pos.y - p2->pos.y
    };
    float dist2 = dp.x * dp.x + dp.y * dp.y;
    if (dist2 < eps) {
        p1->vel.x = GRAVITY_MAGNITUDE * ((float)(rand() % 20000) / 10000.0 - 1.0);
        p1->vel.y = GRAVITY_MAGNITUDE * ((float)(rand() % 20000) / 10000.0 - 1.0);
        p2->vel.x = GRAVITY_MAGNITUDE * ((float)(rand() % 20000) / 10000.0 - 1.0);
        p2->vel.y = GRAVITY_MAGNITUDE * ((float)(rand() % 20000) / 10000.0 - 1.0);
		//p1->vel.x *= -1;
		//p1->vel.y *= -1;
    }
    if (dist2 < minDist * minDist) {
        float dist = sqrtf(dist2);
        float force = k * (minDist - dist) / dist;

        Vec2 dir = { dp.x / dist, dp.y / dist };
        p1->vel.x += dir.x * force * DT;
        p1->vel.y += dir.y * force * DT;
        p2->vel.x -= dir.x * force * DT;
        p2->vel.y -= dir.y * force * DT;
    }
}

void apply_repulsion_optimized_without_hash() {
    for (int i = 0; i < REPULSION_GRID_DIM; i++) {
		for (int j = 0; j < REPULSION_GRID_DIM; j++) {
			indexTable[i][j] = 0;
		}
    }
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        Particle* p = particles + i;
        if (isnan(p->pos.x) || isnan(p->pos.y)) {
            p->pos.x = 0;
            p->pos.y = 0;
            p->vel.x = 0;
            p->vel.y = 0;
        }
        int x = (int)(p->pos.x / minDist);
        int y = (int)(p->pos.y / minDist);
		indexTable[x][y]++;
    }

    int last_val = 0;
	for (int i = 0; i < REPULSION_GRID_DIM; i++) {
		for (int j = 0; j < REPULSION_GRID_DIM; j++) {
			indexTable[i][j] += last_val;
			last_val = indexTable[i][j];
		}
	}

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        Particle* p = particles + i;
        int x = (int)(p->pos.x / minDist);
        int y = (int)(p->pos.y / minDist);
        int final_index = --indexTable[x][y];
        particle_lookup[final_index] = p;
    }
    for (int i1 = 0; i1 < REPULSION_GRID_DIM; i1++) {
        for (int j1 = 0; j1 < REPULSION_GRID_DIM; j1++) {
            for (int i2 = i1 - 1; i2 <= i1 + 1 && i2 < REPULSION_GRID_DIM; i2++) {
                if (i2 < 0) continue;
                for (int j2 = j1 - 1; j2 <= j1 + 1 && j2 < REPULSION_GRID_DIM; j2++) {
                    if (j2 < 0) continue;
                    int lookup_index1 = indexTable[i1][j1];
                    while (lookup_index1 >= 0
                        && lookup_index1 < PARTICLE_COUNT
                        && (int)(particle_lookup[lookup_index1]->pos.x / minDist) == i1
                        && (int)(particle_lookup[lookup_index1]->pos.y / minDist) == j1) {
                        int lookup_index2 = indexTable[i2][j2];
                        while (lookup_index2 >= 0
                            && lookup_index2 < PARTICLE_COUNT
                            && (int)(particle_lookup[lookup_index2]->pos.x / minDist) == i2
                            && (int)(particle_lookup[lookup_index2]->pos.y / minDist) == j2) {
                            if (particle_lookup[lookup_index1] != particle_lookup[lookup_index2]) {
                                apply_repulsion(particle_lookup[lookup_index1], particle_lookup[lookup_index2]);
                            }
                            lookup_index2++;
                        }
                        lookup_index1++;
                    }

                }
            }
        }
    }
}

void step_simulation() {
	clear_grid();
	particle_to_grid();
	apply_gravity();
	grid_to_particle();
	apply_repulsion_optimized_without_hash();
	//apply_repulsion_optimized();
	//apply_repulsion();
	advect_particles_circular();
	//advect_particles();
}


