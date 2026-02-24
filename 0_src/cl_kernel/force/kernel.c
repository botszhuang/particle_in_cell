#include <cl_version.h>
#include <platform.h>
#include <dimension.h>
#include <force.h>

void create_force_G_kernel ( force_kernel_struct * k,
                                    platform_struct platform , 
                                    cl_uint work_dim,
                                    size_t *global_work_offset,
                                    size_t *global_work_size,
                                    size_t *local_work_size 
                                ){

    cl_int ret = 0 ;

    k->G = clCreateKernel( platform.program , "force_G" , &ret ) ;   CL_CHECK( ret ) ; 
    
    k->work_dim = work_dim ;
    k->global_work_offset = global_work_offset ;
    k->global_work_size   = global_work_size ;
    k->local_work_size    = local_work_size ;

}

void free_force_G_kernel ( force_kernel_struct * k ) {
    if ( k->G ) CL_CHECK ( clReleaseKernel ( k->G ) ) ;
}
void set_force_G_kernel_args ( force_kernel_struct * k , 
                                       cl_mem * force ,
                                       size_t * particle_N ) {

    cl_int ret = 0 ;

    ret = clSetKernelArg( k->G , 0 , sizeof(cl_mem)          , (void*) force      ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->G , 1 , sizeof( particle_N[0] ) , (void*) particle_N ); CL_CHECK( ret ) ;  
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
void run_force_G_kernel ( force_kernel_struct * k , cl_command_queue queue , cl_event_struct waitForEvent, cl_event *event  ){
    run ( k->G);                             
}
#undef run