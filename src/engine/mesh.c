#include <stdlib.h>
#include "mesh.h" 
#include "renderer.h"
#include <string.h>
#include "../config.h"
#include "vertex.h"

Mesh* mesh_create(Vertex* vertices, int vertex_count, int* indices, int index_count) {
    if (vertex_count <= 0 || index_count <= 0) return NULL;

    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) return NULL;

    mesh->vertex_count = vertex_count;
    mesh->index_count = index_count;

    mesh->vertices = (Vertex*)malloc(sizeof(Vertex) * vertex_count);
    if (!mesh->vertices) {
        free(mesh);
        return NULL;
    }
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * vertex_count);

    mesh->indices = (int*)malloc(sizeof(int) * index_count);
    if (!mesh->indices) {
        free(mesh->vertices);
        free(mesh);
        return NULL;
    }
    memcpy(mesh->indices, indices, sizeof(int) * index_count);

    return mesh;
}

void mesh_destroy(Mesh* mesh) {
    if (!mesh) return;

    if (mesh->vertices) free(mesh->vertices);
    if (mesh->indices) free(mesh->indices);

    free(mesh);
}


void mesh_draw(Mesh *mesh) {
    for (int i = 0; i < mesh->index_count; i += 3) {
        Vertex v0 = mesh->vertices[mesh->indices[i]];
        Vertex v1 = mesh->vertices[mesh->indices[i + 1]];
        Vertex v2 = mesh->vertices[mesh->indices[i + 2]];

        int x[3], y[3];
        float z[3];

        // Project vertices
        project_vertex(&v0, screen_width, screen_height, fov, &x[0], &y[0], &z[0]);
        project_vertex(&v1, screen_width, screen_height, fov, &x[1], &y[1], &z[1]);
        project_vertex(&v2, screen_width, screen_height, fov, &x[2], &y[2], &z[2]);

        // Draw the triangle (solid color for now)
        renderer_draw_triangle_filled(
            x[0], y[0], z[0],
            x[1], y[1], z[1],
            x[2], y[2], z[2],
            COLOR_RED  
        );
    }
}

