#pragma once

#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

typedef struct {
    cl_kernel kernel ;

} force_kernel_struct ;

typedef struct {
    unsigned int * grid_N ; 
    cl_mem * gird ;
    unsigned int * particle_N ; 
    cl_mem * particle_X ;
    cl_mem * force ;
    cl_mem * forceOUT ;
} force_kernel_args_struct;

void create_force_kernel   ( cl_kernel * k, platform_struct * platform );
void set_force_kernel_args ( cl_kernel k , force_kernel_args_struct * args ) ;
