/*Random coordinate generator*/
#include <cstdlib>
#include "gen_coords.h"
#include <time.h>

double get_lat() {
    srand(time(0));
    double floor = -8.163333;
    double ceiling = 1.766667;
    double diff = ceiling - floor;
    double rnd;
    double result;
    
    rnd = (double)rand() / RAND_MAX;
    result = floor + rnd * diff;
    return result;
}

double get_long() {
    srand(time(0));
    double floor = 49.883333;
    double ceiling = 60.816667;
    double diff = ceiling - floor;
    double rnd;
    double result;
    
    rnd = (double)rand() / RAND_MAX;
    result = floor + rnd * diff;
    return result;
}

