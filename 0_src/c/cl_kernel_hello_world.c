#ifndef CL_GPU_KERNEL_HELLO_WORLD_C
#define CL_GPU_KERNEL_HELLO_WORLD_C

#include <cl_gpu_profile_struct.h>
#include <cl_erro_code.h>

#define k ( g-> kernel_hello_world )
#define kName ( "hello_world" )

void get_kernel_hello_world ( cl_gpu_profile_struct * g ){
    
    cl_int ret = 0 ;
    k = clCreateKernel(g->program, kName, &ret);
    CL_CHECK ( ret ) ;
    
}
void free_kernel_hello_world ( cl_gpu_profile_struct * g ){
    if ( k ){ CL_CHECK( clReleaseKernel ( k ) ); }
}
void run_kernel_hello_world ( cl_gpu_profile_struct * g ){
    size_t global_item_size = 1;
    size_t local_item_size = 1;
    CL_CHECK( clEnqueueNDRangeKernel( g->queue, 
                                      k , 1, NULL, 
                                 &global_item_size, 
                                 &local_item_size, 
                                 0, NULL, NULL));
}
#undef kName
#undef k
#endif