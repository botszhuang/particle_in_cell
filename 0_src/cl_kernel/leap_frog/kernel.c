#include <cl_version.h>
#include <cl_erro_code.h>
#include <platform.h>
#include <dimension.h>
#include <leap_frog.h>

void create_leap_frog_kernel ( leap_frog_kernel_struct * k , platform_struct * platform  ){

    cl_int ret = 0 ;

    k->X      = clCreateKernel( platform->program , "leap_frog_X"          , &ret ) ;   CL_CHECK( ret ) ; 
    k->V_half = clCreateKernel( platform->program , "leap_frog_V_half"     , &ret ) ;   CL_CHECK( ret ) ; 
    
}

void free_leap_frog_kernel ( leap_frog_kernel_struct * k ) {
    if ( k->X      ) CL_CHECK ( clReleaseKernel ( k->X      ) ) ;
    if ( k->V_half ) CL_CHECK ( clReleaseKernel ( k->V_half ) ) ;
}
void set_leap_frog_kernel_args ( leap_frog_kernel_struct * k , 
                                 leap_frog_arg_struct * args ) {

    cl_int ret = 0 ;
    unsigned int i = 0 ;

    ret = clSetKernelArg( k->X , i++ , sizeof(cl_mem) , (void*) args->position    ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , i++ , sizeof(cl_mem) , (void*) args->velocity    ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , i++ , sizeof(cl_mem) , (void*) args->positionOUT ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , i++ , sizeof(cl_mem) , (void*) args->velocityOUT ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , i++ , sizeof(args->particle_N[0]) , (void*) args->particle_N ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , i++ , sizeof(args->dt[0])         , (void*) args->dt         ); CL_CHECK( ret ) ;

    i = 0 ;
    ret = clSetKernelArg( k->V_half , i++ , sizeof(cl_mem) , (void*) args->velocity    ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , i++ , sizeof(cl_mem) , (void*) args->F           ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , i++ , sizeof(cl_mem) , (void*) args->velocityOUT ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , i++ , sizeof(args->particle_N[0]) , (void*) args->particle_N ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , i++ , sizeof(args->dt[0])         , (void*) args->dt         ); CL_CHECK( ret ) ;

}
void run_leap_frog_init_kernel( leap_frog_kernel_struct * k , myfloat * dt , 
                                  cl_command_queue queue,
                                  cl_uint work_dim,
                                  const size_t *global_work_offset,
                                  const size_t *global_work_size,
                                  const size_t *local_work_size,
                                  cl_uint num_events_in_wait_list,
                                  const cl_event *event_wait_list,
                                  cl_event *event ){
    

    myfloat dt_half = dt [0] * 0.5 ;                                
    CL_CHECK ( clSetKernelArg( k->V_half , 4 , sizeof( dt_half ) , (void*) & dt_half )) ;
    CL_CHECK ( clEnqueueNDRangeKernel( queue, k->V_half , 
                                       work_dim, 
                                       global_work_offset, 
                                       global_work_size, 
                                       local_work_size,
                                       num_events_in_wait_list,
                                       event_wait_list,
                                       event ) ) ;  

    CL_CHECK ( clSetKernelArg( k->V_half , 4 , sizeof(dt[0]) , (void*) dt )) ;
}

