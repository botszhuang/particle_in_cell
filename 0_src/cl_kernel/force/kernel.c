#include <cl_version.h>
#include <platform.h>
#include <dimension.h>
#include <force.h>

void create_force_kernel ( cl_kernel * k , platform_struct * platform ){

    cl_int ret = 0 ;

    * k = clCreateKernel( platform->program , "force" , &ret ) ;   CL_CHECK( ret ) ; 
  
}

void set_force_kernel_args (  cl_kernel kernel , force_kernel_args_struct * k ) {

    const unsigned int cl_mem_size = sizeof ( cl_mem ) ;
    unsigned int i = 0 ;

    CL_CHECK( clSetKernelArg( kernel , i++ , sizeof( k->grid_N[0] )     , (void*) k->grid_N     )) ;  
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                , (void*) k->gird       )) ; 
    CL_CHECK( clSetKernelArg( kernel , i++ , sizeof( k->particle_N[0] ) , (void*) k->particle_N )) ;  
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                , (void*) k->particle_X )) ;
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                , (void*) k->force      )) ;
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                , (void*) k->forceOUT   )) ;  
}

