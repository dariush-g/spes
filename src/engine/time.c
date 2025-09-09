#include "time.h" 



static double last_time = 0.0;


double get_time(void) {
    struct timespec ts;
    
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

double delta_time(void) {
    double current_t = get_time();
    double delta_t = current_t - last_time;
    last_time = current_t;
    return delta_t;
}
