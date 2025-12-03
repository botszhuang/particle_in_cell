#ifndef CL_KERNEL_IS_PARTICLE_IN_CELL_C
#define CL_KERNEL_IS_PARTICLE_IN_CELL_C

#include <cl_gpu_profile_struct.h>
#include <cl_erro_code.h>
#include <test_particle_type.h>
#include <cell_type.h>

#define k ( g-> kernel_is_particle_in_cell )
#define kName ( "is_particle_in_cell" )

void get_kernel_is_particle_in_cell ( cl_gpu_profile_struct * g ){

    cl_int ret = 0 ;
    k = clCreateKernel(g->program, kName, &ret);
    CL_CHECK ( ret ) ;

}
void free_kernel_is_particle_in_cell( cl_gpu_profile_struct * g ){
    if ( k ) { CL_CHECK( clReleaseKernel ( k ) ); }
}
void setArg_for_kernel_is_particle_in_cell ( cell_profile_struct * c , 
                                             test_particle_profile_struct * p ,
                                             cl_gpu_profile_struct * g ) {
    
    const unsigned int sizeof_cl_meme = sizeof ( cl_mem ) ;

    clSetKernelArg( k , 0 , sizeof_cl_meme         , &( c->cl_node  ) );
    clSetKernelArg( k , 1 , sizeof ( c->nodeSize ) , &( c->nodeSize ) );
    clSetKernelArg( k , 2 , sizeof_cl_meme         , &( c->cl_cell  ) );
    clSetKernelArg( k , 3 , sizeof ( c->cellSize ) , &( c->cellSize ) );
    clSetKernelArg( k , 4 , sizeof_cl_meme         , &( p->cl_x     ) );
    clSetKernelArg( k , 5 , sizeof ( p->number   ) , &( p->number   ) );
    clSetKernelArg( k , 6 , sizeof_cl_meme         , &( p->cl_cell_index  ) );
    
}                            

void run_kernel_is_paticle_in_cell( cl_gpu_profile_struct * g ){
    const cl_uint work_dim = 2;
    const size_t global_item_size [ 2 ] = { 64 , 64 } ;
    const size_t local_item_size  [ 2 ] = { 8 , 8 } ; // Must be a divisor of global_item_size
    //Number of Work-Groups = global_work_size / local_work_size
    CL_CHECK( clEnqueueNDRangeKernel( g->queue, 
                                      k , work_dim, NULL, 
                                 global_item_size, 
                                 local_item_size, 
                                 0, NULL, NULL));
}

#undef kName
#undef k
#endif