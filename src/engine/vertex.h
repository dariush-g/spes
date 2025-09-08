#ifndef VERTEX_H
#define VERTEX_H

#include <math.h>

typedef struct {
    float x, y, z;
} Vertex;


void rotate_y(Vertex *v, float angle);

void rotate_x(Vertex *v, float angle);

void project_vertex(Vertex* v, int screen_width, int screen_height, float fov, int *out_x, int *out_y, float *out_depth);

#endif
