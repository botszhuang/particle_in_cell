#include <cl_version.h>
#include <platform.h>
#include <dimension.h>
#include <force.h>

void create_force_G_kernel ( force_kernel_struct * k, platform_struct * platform ){

    cl_int ret = 0 ;

    k->G = clCreateKernel( platform->program , "force_G" , &ret ) ;   CL_CHECK( ret ) ; 
  
}
void free_force_G_kernel ( force_kernel_struct * k ) {
    if ( k->G ) CL_CHECK ( clReleaseKernel ( k->G ) ) ;
}
void set_force_G_kernel_args ( force_kernel_struct * k , 
                                cl_mem * force ,
                                cl_mem * X ,
                                unsigned int * particle_N , 
                                cl_mem * forceOUT
) {

    CL_CHECK( clSetKernelArg( k->G , 0 , sizeof(cl_mem)          , (void*) force      )) ;
    CL_CHECK( clSetKernelArg( k->G , 1 , sizeof(cl_mem)          , (void*) X           )) ;
    CL_CHECK( clSetKernelArg( k->G , 2 , sizeof( particle_N[0] ) , (void*) particle_N )) ;  
    CL_CHECK( clSetKernelArg( k->G , 3 , sizeof(cl_mem)          , (void*) forceOUT   )) ;  
}

