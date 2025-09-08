#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t Color;

#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xFFFFFF
#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF
#define COLOR_YELLOW 0xFFFF00
#define COLOR_CYAN 0x00FFFF
#define COLOR_MAGENTA 0xFF00FF

Color color_rgb(uint8_t r, uint8_t g, uint8_t b);

void color_get_rgb(Color color, uint8_t *r, uint8_t *g, uint8_t *b);

typedef struct {
	uint32_t *pixels;
	float *depth_buffer;
	int width;
	int height;
	int pitch;
} Framebuffer;

int renderer_init(int width, int height);

Framebuffer *renderer_get_framebuffer(void);

void renderer_clear(Color color);

void renderer_clear_depth(void);

void renderer_set_pixel(int x, int y, Color color);

void renderer_set_pixel_depth(int x, int y, float depth, Color color);

void renderer_draw_line(int x0, int y0, int x1, int y1, Color color);

void renderer_draw_triangle_filled(int x0, int y0, float z0, int x1, int y1,
				  float z1, int x2, int y2, float z2,
				  Color color);

void renderer_draw_triangle_wireframe(int x0, int y0, int x1, int y1, int x2,
				      int y2, Color color);

void renderer_present(void);

void renderer_cleanup(void);

#endif
