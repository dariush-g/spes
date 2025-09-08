#include "vertex.h"
#include "math.h"

void rotate_y(Vertex *v, float angle) {
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);

    float x_new = v->x * cos_a + v->z * sin_a;
    float z_new = -v->x * sin_a + v->z * cos_a;

    v->x = x_new;
    v->z = z_new;
}

void rotate_x(Vertex *v, float angle) {
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);

    float y_new = v->y * cos_a - v->z * sin_a;
    float z_new = v->y * sin_a + v->z * cos_a;

    v->y = y_new;
    v->z = z_new;
}


void project_vertex(Vertex *v, int screen_width, int screen_height, float fov,
                    int *out_x, int *out_y, float *out_depth) {
    float aspect = (float)screen_width / (float)screen_height;
    float f = 1.0f / tanf(fov * 0.5f * (M_PI / 180.0f));

    *out_x = (int)((v->x / v->z) * f * aspect * ((float)screen_width/2) + (float)screen_width/2);
    *out_y = (int)((v->y / v->z) * f * ((float)screen_height/2) + (float)screen_height/2);
    *out_depth = v->z;
}

