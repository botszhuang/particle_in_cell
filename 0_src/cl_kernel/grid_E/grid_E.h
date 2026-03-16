#pragma once

#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

typedef struct {
    unsigned int * grid_number ; 
    cl_mem * grid_X ;
    cl_mem * grid_E ;
    unsigned int * particle_number ; 
    cl_mem * particle_X ;
} grid_E_kernel_args_struct;

void create_grid_E_kernel   ( cl_kernel * k, platform_struct * platform );
void set_grid_E_kernel_args ( cl_kernel k , grid_E_kernel_args_struct * args ) ;
