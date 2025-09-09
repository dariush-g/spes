#include "engine/renderer.h"
#include "engine/vertex.h"
#include "engine/window.h"
#include <SDL3/SDL_oldnames.h>
#include "engine/time.h"
#include "engine/math.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


int main(void) {

	if (window_init(800, 600) != 0) {
		return -1;
	}

	renderer_init(800, 600);

	Window *win = window_get();
	printf("Requested: 800x600, Got: %dx%d\n", win->width, win->height);

	Vertex tri[3] = {
	    {-1.0f, -1.0f, 5.0f}, {1.0f, -1.0f, 5.0f}, {0.0f, 1.0f, 5.0f}};

    delta_time(); 
    
	while (!window_should_quit()) {
		window_handle_events();

		renderer_clear(COLOR_WHITE);
		renderer_clear_depth();

		int x[3], y[3];
		float z[3];
		for (int i = 0; i < 3; i++) {
            rotate_x(&tri[i], M_PI / 12);
			project_vertex(&tri[i], 800, 600, 90.0f, &x[i], &y[i],
				       &z[i]);
        }

		renderer_draw_triangle_filled(x[0], y[0], z[0], x[1], y[1],
					      z[1], x[2], y[2], z[2],
					      COLOR_RED);

        
		renderer_present();
		window_present();

                       
        struct timespec req = {0, 16000000}; // 16ms
        nanosleep(&req, NULL);
	}
	renderer_cleanup();
	window_cleanup();

    double dt = delta_time();
    printf("%f", dt);

	return 0;
}
