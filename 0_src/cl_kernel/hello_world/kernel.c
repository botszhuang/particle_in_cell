#include <cl_version.h>

#define kName "hello_world"

static size_t global_item_size = 1 ;
static size_t local_item_size = 1 ;

void create_hello_world_kernel ( cl_kernel * k , cl_program p ){

    cl_int ret = 0 ;
    * k = clCreateKernel( p , kName, &ret ) ;

}

void run_kernel_hello_world ( cl_kernel k , cl_command_queue queue ){

    cl_event event ;

    clEnqueueNDRangeKernel( queue,
                            k , 1, NULL,
                            &global_item_size,
                            &local_item_size,
                            0, NULL, &event );
                            
    clWaitForEvents( 1, &event );
    clReleaseEvent( event );
}
