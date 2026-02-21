#include <cl_platform_struct.h>

void create_queue( cl_platform_struct * a ){
    
    cl_int ret = 0 ;

    a->queue = calloc ( a->device_N , sizeof ( a->queue[0] ) ) ;

    for ( unsigned int i = 0 ; i < a->device_N ; i++ ) {

        #if defined(CL_VERSION_2_0) || (CL_VERSION_3_0)
            //OpenCL 2.0+ headers, preferred
            const cl_queue_properties props[] = { 0 };
                a->queue [ i ] = clCreateCommandQueueWithProperties( a->context, a->devices [ i ], props, &ret);
        #else
            // OpenCL 1.2
            a->queue [ i ] = clCreateCommandQueue( a->context, a->devices [ i ] , 0, &ret); CL_CHECK( ret ) ;
        #endif
    }
}

void finish_queue ( cl_platform_struct * g ){
    for ( unsigned int i = 0 ; i < g->device_N ; i++ ) {
        CL_CHECK( clFlush (g->queue [ i ] ) );
        CL_CHECK( clFinish(g->queue [ i ] ) );
    }
}

