#pragma once

#include <cl_version.h>
#include <dimension.h>

#define particle_dimension dimension_2D_struct

typedef struct{

    unsigned int number ;

    particle_dimension * position ;
    particle_dimension * velocity ;
    particle_dimension * acceleration ;

    unsigned int cl_position_bytes ;
    unsigned int cl_velocity_bytes ;
    unsigned int cl_acceleration_bytes ;
    cl_mem cl_position ;
    cl_mem cl_velocity ;
    cl_mem cl_acceleration ;

} particle_struct ;


