#ifndef MESH_H
#define MESH_H

#include "vertex.h"

typedef struct {
    Vertex *vertices;
    int* indices;
    int vertex_count;
    int index_count;
} Mesh;

Mesh* mesh_create(Vertex *vertices, int vertex_count, int* indices, int index_count);

void mesh_destroy(Mesh *mesh);

Mesh* mesh_load_obj(const char *filepath);

void mesh_draw(Mesh *mesh);

void mesh_rotate(Mesh *mesh, float x_angle, float y_angle, float z_angle);

void mesh_scale(Mesh *mesh, float scale);

void mesh_translate(Mesh *mesh, float dx, float dy, float dz);

void mesh_create_cuboid(float length, float width, float height);

void mesh_create_sphere(float radius);


typedef struct {
    Mesh* mesh;
    float x, y, z; // world position
    float rot_x, rot_y, rot_z; // rotations
    float scale;
} MeshInstance;

void draw_instance(MeshInstance *instance);


#endif
