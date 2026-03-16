#include <platform.h>

static void sub_create_a_queue ( cl_command_queue * q , cl_context context, cl_device_id device , const cl_command_queue_properties properties ) {
    const cl_queue_properties props[] = { CL_QUEUE_PROPERTIES, properties, 0 } ; // OpenCL 2.0+ , preferred

    cl_int ret = 0 ;

    #if defined(CL_VERSION_2_0) || (CL_VERSION_3_0)
            //OpenCL 2.0+ headers, preferred
        *q = clCreateCommandQueueWithProperties( context, device , props, &ret); CL_CHECK ( ret ) ;
    #else
            // OpenCL 1.2
        *q = clCreateCommandQueue( context, devices, properties, &ret); CL_CHECK( ret ) ;
    #endif
}

void create_queue_in_order ( cl_command_queue * q , cl_context context, cl_device_id device ){
    
    sub_create_a_queue ( q , context, device , CL_QUEUE_PROFILING_ENABLE ) ; 
    
}
void create_queue_out_of_order ( cl_command_queue * q , cl_context context, cl_device_id device ){

    sub_create_a_queue ( q , context, device , CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ) ; 
   
}
