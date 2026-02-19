#pragma once
#include <cl_version.h>

#define free_k(x) { if ( x ){ clReleaseKernel ( x ) ; } }

void create_hello_world_kernel ( cl_kernel * k , cl_program p ) ;
void run_kernel_hello_world ( cl_kernel k , cl_command_queue queue ) ;