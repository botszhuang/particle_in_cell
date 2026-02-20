#pragma once

#include <dimension.h>

#define particle_dimension dimension_2D_struct

typedef struct{

    unsigned int number ;

    particle_dimension * position ;
    particle_dimension * velocity ;
    particle_dimension * acceleration ;

} particle_struct ;