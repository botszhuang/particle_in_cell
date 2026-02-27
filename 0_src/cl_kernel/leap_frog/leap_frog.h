#pragma once

#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

typedef struct {
    cl_kernel X ;
    cl_kernel V_half ;
    cl_uint work_dim;
    size_t *global_work_offset;
    size_t *global_work_size;
    size_t *local_work_size;
} leap_frog_kernel_struct ;

void create_leap_frog_kernel ( leap_frog_kernel_struct * k,
                                    platform_struct platform , 
                                    cl_uint work_dim,
                                    size_t *global_work_offset,
                                    size_t *global_work_size,
                                    size_t *local_work_size 
                                );
void free_leap_frog_kernel ( leap_frog_kernel_struct * k ) ;
void set_leap_frog_kernel_args ( leap_frog_kernel_struct * k , 
                                       cl_mem * position ,
                                       cl_mem * velocity ,
                                       cl_mem * acc ,
                                       size_t * particle_N , 
                                       myfloat * dt ) ;
void run_init_leap_frog_V_half_kernel ( leap_frog_kernel_struct * k , cl_command_queue queue , cl_event_struct wait, cl_event * event , myfloat * dt ) ;
void run_leap_frog_X_kernel      ( leap_frog_kernel_struct * k , cl_command_queue queue , cl_event_struct wait, cl_event *event  ) ;
void run_leap_frog_V_half_kernel ( leap_frog_kernel_struct * k , cl_command_queue queue , cl_event_struct wait, cl_event *event  ) ;