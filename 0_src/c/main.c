
#include <stdlib.h>

#include <c_tool.h>
#include <test_particle_run.h>

#include <cl_gpu_tool.h>
#include <cl_kernel.h>
#include <cell_run.h>
#include <read_data.h>

int main(){

  
    cl_gpu_profile_struct * g = calloc ( 1 , sizeof ( cl_gpu_profile_struct ) )  ;   
    get_platform_number_and_ids ( g ) ;                                
    get_context_and_queue ( g ) ;

    get_program ( g ) ;
    get_kernel_hello_world  ( g ) ;
    run_kernel_hello_world ( g ) ;

    get_kernel_leapfrog_step ( g ) ;
    get_kernel_is_particle_in_cell ( g ) ;

    t_type t = 0 ;
    test_particle_profile_struct * p ;
    init_test_particle_mem  ( & p  ) ;
/*    init_test_particle_status ( p ) ;
    cl_init_test_particle_mem ( p , g );
    print_test_particle ( p ) ;

    cell_profile_struct * cell = malloc ( sizeof ( cell [0] ) ) ;
    read_node ( cell ) ;
    //print_node_in_cell ( cell ) ;
    read_cell ( cell ) ;
    //print_node_index_of_cell ( cell ) ;
    cl_cell_mem_init ( cell , g ) ;

    
    setArg_for_kernel_leapfrog_step ( p , g , &t ) ;
    setArg_for_kernel_is_particle_in_cell ( cell , p , g ) ;
    
    // write data to GPU
    host_to_gpu ( p , g , x ) ;
    host_to_gpu ( p , g , v ) ;
    host_to_gpu ( p , g , F ) ;
    host_to_gpu ( p , g , m ) ;
    host_to_gpu ( p , g , q ) ;
    host_to_gpu ( cell , g , node ) ;
    host_to_gpu ( cell , g , cell ) ;

    run_kernel_is_paticle_in_cell ( g ) ;
    //run_kernel_leapfrog_step0 ( g ) ;
    //run_kernel_leapfrog_step1 ( g ) ;

    // Read data from GPU
    gpu_to_host ( p , g , x ) ;
    gpu_to_host ( p , g , v ) ;
    gpu_to_host ( p , g , F ) ;
    gpu_to_host ( p , g , m ) ;
    gpu_to_host ( p , g , q ) ;

    print_test_particle ( p ) ;

    // Clean up
    flush_and_finish_queue ( g ) ;
    free_kernel_hello_world ( g );
    free_kernel_is_particle_in_cell ( g ) ;
    free_kernel_leapfrog_step ( g );
*/    
    free_program  ( g );
    free_queue    ( g );

//    cl_free_test_particle_mem ( p );

    free_context  ( g );
    free_device   ( g );

    free_platform ( g );
    iffree ( g ) ;

//    free_cell_profile ( cell ) ;
    free_test_particle_mem ( p ) ;

    return EXIT_SUCCESS;
}
