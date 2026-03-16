#pragma once

#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

typedef struct {
    cl_kernel X ;
    cl_kernel V_half ;
} leap_frog_kernel_struct ;
typedef struct {
    cl_mem * position ;
    cl_mem * velocity ;
    cl_mem * F ;
    unsigned int * particle_N ; 
    myfloat * dt ;
    cl_mem * positionOUT ;
    cl_mem * velocityOUT ; 
}leap_frog_arg_struct ;

void create_leap_frog_kernel   ( leap_frog_kernel_struct * k, platform_struct * platform );
void free_leap_frog_kernel     ( leap_frog_kernel_struct * k ) ;
void set_leap_frog_kernel_args ( leap_frog_kernel_struct * k , leap_frog_arg_struct * args  ) ;
void run_leap_frog_init_kernel( leap_frog_kernel_struct * k , myfloat * dt , 
                                  cl_command_queue queue,
                                  cl_uint work_dim,
                                  const size_t *global_work_offset,
                                  const size_t *global_work_size,
                                  const size_t *local_work_size,
                                  cl_uint num_events_in_wait_list,
                                  const cl_event *event_wait_list,
                                  cl_event *event ) ;
