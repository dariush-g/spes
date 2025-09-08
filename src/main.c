#include "engine/renderer.h"
#include "engine/window.h"
#include <SDL3/SDL_oldnames.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {

	if (window_init(800, 600) != 0) {
		return -1;
	}

	renderer_init(800, 600);

	Window *win = window_get();
	printf("Requested: 800x600, Got: %dx%d\n", win->width, win->height);

	while (!window_should_quit()) {
		window_handle_events();

		renderer_clear(COLOR_WHITE);
		renderer_clear_depth();

		renderer_draw_triangle_filled(200, 100, 1.0f, 200, 499, 1.0f,
					      600, 499, 1.0f, COLOR_RED);	
		renderer_present();
		window_present();
		SDL_Delay(16);
	}
	renderer_cleanup();
	window_cleanup();

	return 0;
}
