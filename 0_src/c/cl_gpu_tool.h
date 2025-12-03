#ifndef CL_GPU_TOOL_H
#define CL_GPU_TOOL_H


#include <cl_version.h>
#include <cl_gpu_profile_struct.h>
#include <cl_erro_code.h>

#include <stdlib.h>

#define cl_iffree(a) if (a) { clReleaseMemObject(a); a = NULL; }

void get_platform_number_and_ids( cl_gpu_profile_struct * g ) ;
void get_platform_info(  const cl_platform_id id ) ;
void free_platform ( cl_gpu_profile_struct * g ) ;

cl_device_id * get_devices_info(  unsigned int * divce_numPtr , cl_platform_id platforms  ) ;
void get_device_ids( cl_gpu_profile_struct * g ) ;
void free_device ( cl_gpu_profile_struct * g ) ;

void get_context_and_queue ( cl_gpu_profile_struct * a ) ;
void free_context ( cl_gpu_profile_struct * a ) ;
void free_queue   ( cl_gpu_profile_struct * a ) ;
void flush_and_finish_queue ( cl_gpu_profile_struct * g ) ;

void get_program ( cl_gpu_profile_struct * a ) ;
void free_program ( cl_gpu_profile_struct * a ) ;


void get_kernel  ( cl_gpu_profile_struct * g ) ;
void free_kernel ( cl_gpu_profile_struct * g ) ;

#endif
