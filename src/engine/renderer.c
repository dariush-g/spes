#include "renderer.h"
#include "window.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Framebuffer g_framebuffer = {0};

static void swap_int(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

static void swap_float(float *a, float *b) {
	float temp = *a;
	*a = *b;
	*b = temp;
}

Color color_rgb(uint8_t r, uint8_t g, uint8_t b) {
	return (r << 16) | (g << 8) | b;
}

void color_get_rbg(Color color, uint8_t *r, uint8_t *g, uint8_t *b) {
	*r = (color >> 16) & 0xFF;
	*g = (color >> 8) & 0xFF;
	*b = color & 0xFF;
}

int renderer_init(int width, int height) {
	g_framebuffer.width = width;
	g_framebuffer.height = height;
	g_framebuffer.pitch = width * sizeof(uint32_t);

	g_framebuffer.pixels = malloc(width * height * sizeof(uint32_t));
	if (!g_framebuffer.pixels) {
		fprintf(stderr, "Failed to allocate pixel buffer\n");
		return -1;
	}

	g_framebuffer.depth_buffer = malloc(width * height * sizeof(float));
	if (!g_framebuffer.depth_buffer) {
		fprintf(stderr, "Failed to allocate depth buffer\n");
		free(g_framebuffer.pixels);
		return -1;
	}

	printf("Renderer initialized: %dx%d\n", width, height);
	return 0;
}

Framebuffer *renderer_get_framebuffer(void) { return &g_framebuffer; }

void renderer_clear(Color color) {
	for (int i = 0; i < g_framebuffer.width * g_framebuffer.height; i++) {
		g_framebuffer.pixels[i] = color;
	}
}

void renderer_clear_depth(void) {
	for (int i = 0; i < g_framebuffer.width * g_framebuffer.height; i++) {
		g_framebuffer.depth_buffer[i] = INFINITY;
	}
}

void renderer_set_pixel(int x, int y, Color color) {
	if (x >= 0 && x < g_framebuffer.width && y >= 0 &&
	    y < g_framebuffer.height) {
		g_framebuffer.pixels[y * g_framebuffer.width + x] = color;
	}
}

void renderer_set_pixel_depth(int x, int y, float depth, Color color) {
	if (x >= 0 && x < g_framebuffer.width && y >= 0 &&
	    y < g_framebuffer.height) {
		int index = y * g_framebuffer.width + x;
		if (depth < g_framebuffer.depth_buffer[index]) {
			g_framebuffer.depth_buffer[index] = depth;
			g_framebuffer.pixels[index] = color;
		}
	}
}

void renderer_draw_line(int x0, int y0, int x1, int y1, Color color) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;

	int x = x0, y = y0;

	while (true) {
		renderer_set_pixel(x, y, color);

		if (x == x1 && y == y1)
			break;

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x += sx;
		}
		if (e2 < dx) {
			err += dx;
			y += sy;
		}
	}
}

void renderer_draw_triangle_wireframe(int x0, int y0, int x1, int y1, int x2,
				      int y2, Color color) {
	renderer_draw_line(x0, y0, x1, y1, color);
	renderer_draw_line(x1, y1, x2, y2, color);
	renderer_draw_line(x2, y2, x0, y0, color);
}

void renderer_draw_triangle_filled(int x0, int y0, float z0, int x1, int y1,
				   float z1, int x2, int y2, float z2,
				   Color color) {
	if (y1 < y0) {
		swap_int(&x0, &x1);
		swap_int(&y0, &y1);
		swap_float(&z0, &z1);
	}
	if (y2 < y0) {
		swap_int(&x0, &x2);
		swap_int(&y0, &y2);
		swap_float(&z0, &z2);
	}
	if (y2 < y1) {
		swap_int(&x1, &x2);
		swap_int(&y1, &y2);
		swap_float(&z1, &z2);
	}

	if (y0 == y2)
		return;

	for (int y = y0; y <= y2; y++) {
		if (y < 0 || y >= g_framebuffer.height)
			continue;

		int x_start, x_end;
		float z_start, z_end;

		if (y <= y1) {
			if (y1 == y0)
				continue;

			float t1 = (float)(y - y0) / (y1 - y0);
			float t2 = (float)(y - y0) / (y2 - y0);

			x_start = (int)(x0 + t1 * (x1 - x0));
			x_end = (int)(x0 + t2 * (x2 - x0));

			z_start = z0 + t1 * (z1 - z0);
			z_end = z0 + t2 * (z2 - z0);
		} else {
			float t1 = (float)(y - y1) / (y2 - y1);
			float t2 = (float)(y - y0) / (y2 - y0);

			x_start = (int)(x1 + t1 * (x2 - x1));
			x_end = (int)(x0 + t2 * (x2 - x0));
			z_start = z1 + t1 * (z2 - z1);
			z_end = z0 + t2 * (z2 - z0);
		}

		if (x_start > x_end) {
			swap_int(&x_start, &x_end);
			swap_float(&z_start, &z_end);
		}

		for (int x = x_start; x <= x_end; x++) {
			if (x >= 0 && x < g_framebuffer.width) {
				float t = (x_end == x_start)
					      ? 0.0f
					      : (float)(x - x_start) /
						    (x_end - x_start);
				float depth = z_start + t * (z_end - z_start);
				renderer_set_pixel_depth(x, y, depth, color);
			}
		}
	}
}

void renderer_present(void) {
	Window *window = window_get();
	if (!window || !window->surface)
		return;

	memcpy(window->surface->pixels, g_framebuffer.pixels,
	       g_framebuffer.width * g_framebuffer.height * sizeof(uint32_t));
}

void renderer_cleanup(void) {
	if (g_framebuffer.pixels) {
		free(g_framebuffer.pixels);
		g_framebuffer.pixels = NULL;
	}
	if (g_framebuffer.depth_buffer) {
		free(g_framebuffer.depth_buffer);
		g_framebuffer.depth_buffer = NULL;
	}
	printf("Renderer cleaned up\n");
}
