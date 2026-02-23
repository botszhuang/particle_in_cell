#include <cl_version.h>
#include <platform.h>
#include <dimension.h>

#define kName "particle_pusher"

void create_particle_pusher_kernel ( lauch_kernel_struct * k,
                                    platform_struct platform , 
                                    cl_uint work_dim,
                                    size_t *global_work_offset,
                                    size_t *global_work_size,
                                    size_t *local_work_size 
                                ){

    cl_int ret = 0 ;

    k->kernel = clCreateKernel( platform.program , kName, &ret ) ;   CL_CHECK( ret ) ; 
    
    k->work_dim = work_dim ;
    k->global_work_offset = global_work_offset ;
    k->global_work_size   = global_work_size ;
    k->local_work_size    = local_work_size ;

}

void set_particle_pusher_kernel_args ( lauch_kernel_struct * k , 
                                       cl_mem * position ,
                                       cl_mem * velocity ,
                                       cl_mem * force ,
                                       size_t * particle_N , 
                                       myfloat * dt ) {

    cl_int ret = 0 ;

    ret = clSetKernelArg( k->kernel , 0 , sizeof(cl_mem)          , (void*) position   ); CL_CHECK( ret ) ;    
    ret = clSetKernelArg( k->kernel , 1 , sizeof(cl_mem)          , (void*) velocity   ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->kernel , 2 , sizeof(cl_mem)          , (void*) force      ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->kernel , 3 , sizeof( particle_N[0] ) , (void*) particle_N ); CL_CHECK( ret ) ;
    ret = clSetKernelArg( k->kernel , 4 , sizeof( dt[0]         ) , (void*) dt         ); CL_CHECK( ret ) ;

}

