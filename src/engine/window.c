#include "window.h"
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

static Window g_window = {0};

int window_init(int width, int height) {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}

	g_window.window = SDL_CreateWindow("Spes", width, height, 0);

	if (g_window.window) {
		SDL_SetWindowSize(g_window.window, width, height);
		SDL_SetWindowPosition(g_window.window, SDL_WINDOWPOS_CENTERED,
				      SDL_WINDOWPOS_CENTERED);
	}

	if (!g_window.window) {
		fprintf(stderr, "SDL_CreateWindow failed: %s\n",
			SDL_GetError());
		exit(-1);
	}

	g_window.surface = SDL_GetWindowSurface(g_window.window);

	if (!g_window.surface) {
		fprintf(stderr, "SDL_GetWindowSurface failed: %s\n",
			SDL_GetError());
		SDL_DestroyWindow(g_window.window);
		SDL_Quit();
		return -1;
	}

	g_window.width = width;
	g_window.height = height;
	g_window.should_quit = false;

	printf("Window initialized: %dx%d\n", width, height);

	return 0;
}

Window *window_get(void) { return &g_window; }

void window_handle_events(void) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			g_window.should_quit = true;
			break;

		case SDL_EVENT_KEY_DOWN:
			if (event.key.key == SDLK_ESCAPE) {
				g_window.should_quit = true;
			}
			break;

		case SDL_EVENT_WINDOW_RESIZED:
			g_window.width = event.window.data1;
			g_window.height = event.window.data2;
			// Surface will be updated automatically
			g_window.surface =
			    SDL_GetWindowSurface(g_window.window);
			printf("Window resized: %dx%d\n", g_window.width,
			       g_window.height);
			break;
		}
	}
}

void window_present(void) {
	if (!SDL_UpdateWindowSurface(g_window.window)) {
		fprintf(stderr, "SDL_UpdateWindowSurface failed: %s\n",
			SDL_GetError());
	}
}

bool window_should_quit(void) { return g_window.should_quit; }

void window_cleanup(void) {
	if (g_window.window) {
		SDL_DestroyWindow(g_window.window);
		g_window.window = NULL;
	}

	SDL_Quit();
	printf("Window cleaned up\n");
}
