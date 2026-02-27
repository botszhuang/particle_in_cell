#pragma once

#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

typedef struct {
    cl_kernel G ;
} force_kernel_struct ;

void create_force_G_kernel   ( force_kernel_struct * k, platform_struct * platform );
void free_force_G_kernel     ( force_kernel_struct * k ) ;
void set_force_G_kernel_args ( force_kernel_struct * k , cl_mem * force , cl_mem * X , unsigned int * particle_N , cl_mem * forceOUT) ;
