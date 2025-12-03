#ifndef CL_KERNEL_H
#define CL_KERNEL_H

#include <cl_gpu_profile_struct.h>
#include <test_particle_type.h>
#include <cell_type.h>

void get_kernel_hello_world  ( cl_gpu_profile_struct * g ) ;
void free_kernel_hello_world ( cl_gpu_profile_struct * g ) ;
void run_kernel_hello_world  ( cl_gpu_profile_struct * g ) ;

void get_kernel_leapfrog_step  ( cl_gpu_profile_struct * g ) ;
void free_kernel_leapfrog_step ( cl_gpu_profile_struct * g ) ;
void setArg_for_kernel_leapfrog_step ( test_particle_profile_struct * p , cl_gpu_profile_struct * g , t_type * t ) ;
void run_kernel_leapfrog_step0  ( cl_gpu_profile_struct * g ) ;
void run_kernel_leapfrog_step1  ( cl_gpu_profile_struct * g ) ;

void get_kernel_is_particle_in_cell ( cl_gpu_profile_struct * g ) ;
void free_kernel_is_particle_in_cell( cl_gpu_profile_struct * g ) ;
void setArg_for_kernel_is_particle_in_cell ( cell_profile_struct * c , 
                                             test_particle_profile_struct * p ,  
                                             cl_gpu_profile_struct * g ) ;             
void run_kernel_is_paticle_in_cell( cl_gpu_profile_struct * g ) ;


#endif