#include <cl_version.h>
#include <platform.h>
#include <dimension.h>
#include <grid_E.h>

void create_grid_E_kernel ( cl_kernel * k , platform_struct * platform ){

    cl_int ret = 0 ;

    * k = clCreateKernel( platform->program , "grid_E" , &ret ) ;   CL_CHECK( ret ) ; 
  
}

void set_grid_E_kernel_args (  cl_kernel kernel , grid_E_kernel_args_struct * k ) {

    const unsigned int cl_mem_size = sizeof ( cl_mem ) ;
    unsigned int i = 0 ;

    CL_CHECK( clSetKernelArg( kernel , i++ , sizeof( k->grid_number[0] )     , (void*) k->grid_number  )) ;  
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                     , (void*) k->grid_X       )) ; 
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                     , (void*) k->grid_E       )) ; 
    CL_CHECK( clSetKernelArg( kernel , i++ , sizeof( k->particle_number[0] ) , (void*) k->particle_number )) ;  
    CL_CHECK( clSetKernelArg( kernel , i++ , cl_mem_size                     , (void*) k->particle_X )) ;
}

