#ifndef CL_GPU_CONTEXT_AND_QUEUE_C
#define CL_GPU_CONTEXT_AND_QUEUE_C
#include <stdio.h>
#include <cl_gpu_profile_struct.h>
#include <cl_erro_code.h>

void get_context_and_queue( cl_gpu_profile_struct * a ){

    cl_uint ret = 0 ;

    const unsigned int index = 0 ;

    a->context = clCreateContext(NULL, 1,&( a->device_Ids[ index ]) , NULL, NULL, &ret);
    CL_CHECK (ret) ;
    
    #if defined(CL_VERSION_2_0) || (CL_VERSION_3_0)
        //OpenCL 2.0+ headers, preferred
        const cl_queue_properties props[] = { 0 };
        a->queue = clCreateCommandQueueWithProperties(a->context, a->device_Ids[ index ], props, &ret);
    #else
        // OpenCL 1.2
        a->queue = clCreateCommandQueue(a->context, a->device_Ids [ index ], 0, &ret);
    #endif
        CL_CHECK ( ret ) ; 
}

void free_context ( cl_gpu_profile_struct * g ) { 
    if( g->context ){
        CL_CHECK ( clReleaseContext(g->context) ) ; 
    }
}
void free_queue ( cl_gpu_profile_struct * g ) { 
    if( g->queue ) {
        CL_CHECK ( clReleaseCommandQueue(g->queue) ) ;
    } 
}
void flush_and_finish_queue ( cl_gpu_profile_struct * g ){
    CL_CHECK( clFlush (g->queue));
    CL_CHECK( clFinish(g->queue));
}
#endif