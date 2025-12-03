#ifndef CL_GPU_PROFILE_STRUCT_H
#define CL_GPU_PROFILE_STRUCT_H

#include <cl_version.h>

typedef struct{
    unsigned int platform_id_num ;
    cl_platform_id platform_Ids ;                                

    unsigned int device_number ;
    cl_device_id * device_Ids ;

    cl_context context ;
    cl_command_queue queue ;
    cl_program program ;
    cl_kernel kernel_hello_world ;
    cl_kernel kernel_leapfrog_step0 ;
    cl_kernel kernel_leapfrog_step1 ;
    cl_kernel kernel_is_particle_in_cell ;
    
}cl_gpu_profile_struct;

#endif
