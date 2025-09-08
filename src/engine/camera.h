#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>


typedef struct {
    float x, y, z;
    float pitch, yaw, roll;
} Camera;

static inline Camera camera_create(float x, float y, float z,
                                   float pitch, float yaw, float roll) {
    Camera cam;
    cam.x = x; cam.y = y; cam.z = z;
    cam.pitch = pitch; cam.yaw = yaw; cam.roll = roll;
    return cam;
}

// Transform a point from world space to camera space
static inline void camera_transform_vertex(const Camera *cam,
                                           float vx, float vy, float vz,
                                           float *out_x, float *out_y, float *out_z) {
    // Translate vertex by camera position
    float x = vx - cam->x;
    float y = vy - cam->y;
    float z = vz - cam->z;

    // Apply yaw (around Y axis)
    float cos_y = cosf(-cam->yaw);
    float sin_y = sinf(-cam->yaw);
    float x1 = cos_y * x + sin_y * z;
    float z1 = -sin_y * x + cos_y * z;
    x = x1; z = z1;

    // Apply pitch (around X axis)
    float cos_p = cosf(-cam->pitch);
    float sin_p = sinf(-cam->pitch);
    float y1 = cos_p * y - sin_p * z;
    float z2 = sin_p * y + cos_p * z;
    y = y1; z = z2;

    // Roll (around Z axis)
    float cos_r = cosf(-cam->roll);
    float sin_r = sinf(-cam->roll);
    float x2 = cos_r * x - sin_r * y;
    float y2 = sin_r * x + cos_r * y;
    x = x2; y = y2;

    // Output
    *out_x = x;
    *out_y = y;
    *out_z = z;
}

#endif
