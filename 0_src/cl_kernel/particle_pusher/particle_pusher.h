#pragma once

#include <cl_version.h>
#include <platform.h>

void create_particle_pusher_kernel ( lauch_kernel_struct * k,
                                    platform_struct platform , 
                                    cl_uint work_dim,
                                    size_t *global_work_offset,
                                    size_t *global_work_size,
                                    size_t *local_work_size 
                                );
void set_particle_pusher_kernel_args ( lauch_kernel_struct * k , 
                                       cl_mem * position ,
                                       cl_mem * velocity ,
                                       cl_mem * force ,
                                       const size_t * particle_N ) ;