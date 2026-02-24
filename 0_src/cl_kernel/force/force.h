#pragma once

#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

typedef struct {
    cl_kernel G ;
    cl_uint work_dim;
    size_t *global_work_offset;
    size_t *global_work_size;
    size_t *local_work_size;
} force_kernel_struct ;

void create_force_G_kernel ( force_kernel_struct * k,
                                    platform_struct platform , 
                                    cl_uint work_dim,
                                    size_t *global_work_offset,
                                    size_t *global_work_size,
                                    size_t *local_work_size 
                                );
void free_force_G_kernel ( force_kernel_struct * k ) ;
void set_force_G_kernel_args ( force_kernel_struct * k , cl_mem * force , size_t * particle_N ) ;
void run_force_G_kernel ( force_kernel_struct * k , cl_command_queue queue , cl_event_struct waitForEvent, cl_event *event  ) ;
