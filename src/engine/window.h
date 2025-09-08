#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_Window *window;
	SDL_Surface *surface;

	int width;
	int height;
	int original_width;
	int original_height;

	bool should_quit;
	bool is_fullscreen;
	bool is_focused;

	uint32_t *pixels;
	int pitch; // bytes per row

	uint32_t last_frame_time;
	float delta_time;

	bool keys[SDL_SCANCODE_COUNT];
	int mouse_x, mouse_y;
	uint32_t mouse_buttons;
} Window;

int window_init(int width, int height);

Window *window_get(void);

void window_handle_events(void);

// update display
void window_present(void);

bool window_should_quit(void);

void window_cleanup(void);

#endif
