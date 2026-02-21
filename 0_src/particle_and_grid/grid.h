#pragma once
#include <cl_version.h>
#include <dimension.h>

#define grid_dimension dimension_2D_struct

typedef struct{

    unsigned int number;
    grid_dimension * position ;

    unsigned int cl_position_bytes ;
    cl_mem cl_position ;

} grid_struct ;

