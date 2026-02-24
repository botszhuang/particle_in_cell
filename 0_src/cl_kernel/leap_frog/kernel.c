#include <cl_version.h>
#include <cl_erro_code.h>
#include <platform.h>
#include <dimension.h>
#include <leap_frog.h>

void create_leap_frog_kernel ( leap_frog_kernel_struct * k,
                                    platform_struct platform , 
                                    cl_uint work_dim,
                                    size_t *global_work_offset,
                                    size_t *global_work_size,
                                    size_t *local_work_size 
                                ){

    cl_int ret = 0 ;

    k->X           = clCreateKernel( platform.program , "leap_frog_X"          , &ret ) ;   CL_CHECK( ret ) ; 
    k->V_half      = clCreateKernel( platform.program , "leap_frog_V_half"     , &ret ) ;   CL_CHECK( ret ) ; 
    
    k->work_dim = work_dim ;
    k->global_work_offset = global_work_offset ;
    k->global_work_size   = global_work_size ;
    k->local_work_size    = local_work_size ;

}

void free_leap_frog_kernel ( leap_frog_kernel_struct * k ) {
    if ( k->X      ) CL_CHECK ( clReleaseKernel ( k->X      ) ) ;
    if ( k->V_half ) CL_CHECK ( clReleaseKernel ( k->V_half ) ) ;
}
void set_leap_frog_kernel_args ( leap_frog_kernel_struct * k , 
                                       cl_mem * position ,
                                       cl_mem * velocity ,
                                       cl_mem * F ,
                                       size_t * particle_N , 
                                       myfloat * dt ) {

    cl_int ret = 0 ;

    ret = clSetKernelArg( k->X , 0 , sizeof(cl_mem) , (void*) position   ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , 1 , sizeof(cl_mem) , (void*) velocity   ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , 2 , sizeof(particle_N[0]) , (void*) particle_N ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->X , 3 , sizeof(dt[0]) , (void*) dt                 ); CL_CHECK( ret ) ;

    ret = clSetKernelArg( k->V_half , 0 , sizeof(cl_mem) , (void*) velocity   ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , 1 , sizeof(cl_mem) , (void*) F          ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , 2 , sizeof(particle_N[0]) , (void*) particle_N ) ; CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->V_half , 3 , sizeof(dt[0]) , (void*) dt                 ); CL_CHECK( ret ) ;

}

#define run(kernel) {   CL_CHECK ( clEnqueueNDRangeKernel( queue,\
                             kernel, \
                             k->work_dim, \
                             k->global_work_offset,\
                             k->global_work_size,\
                             k->local_work_size,\
                             waitForEvent.count,\
                             waitForEvent.array,\
                             event ) ) ;}
void run_init_leap_frog_V_half_kernel ( leap_frog_kernel_struct * k , myfloat dt , cl_command_queue queue , cl_event_struct waitForEvent, cl_event *event  ){

    myfloat dt_half = dt * 0.5 ;
   
    cl_int ret = 0 ;

    ret = clSetKernelArg( k->X , 3 , sizeof( dt_half ) , (void*) &dt_half ); CL_CHECK( ret ) ;

    run ( k->V_half );                             
   
    ret = clSetKernelArg( k->X , 3 , sizeof ( dt ) , (void*) &dt ); CL_CHECK( ret ) ;



}
void run_leap_frog_X_kernel      ( leap_frog_kernel_struct * k , cl_command_queue queue , cl_event_struct waitForEvent, cl_event *event  ){
    run ( k->X);                             
}
void run_leap_frog_V_half_kernel ( leap_frog_kernel_struct * k , cl_command_queue queue , cl_event_struct waitForEvent, cl_event *event  ){
    run ( k->V_half );                             
}
#undef run