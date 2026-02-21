
#include <cl_platform_struct.h>
#include <cl_kernel_list.h>
#include <grid.h>
#include <particle.h>
#include <get_input_info.h>

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
  
    printf("Hello world!\n") ;

    // initialize the platform
    print_all_platform_info () ;

    cl_platform_struct gpu ;

    select_the_platform ( &gpu ) ;

    get_device_ids ( gpu.platform , & gpu.devices , & gpu.device_N ) ;
    list_devices( gpu.platform , true ) ;

    create_context ( &gpu );
    create_queue   ( &gpu );

    init_program ( &gpu );

    cl_kernel hello_world_kernel ;
    create_hello_world_kernel ( &hello_world_kernel , gpu.program ) ;
    run_kernel_hello_world ( hello_world_kernel , gpu.queue[0] ) ;

    
    // read input.tex
    input_tex_tag_struct input_tag ;
    get_input_tex ( &input_tag , inputFile ) ;


    grid_struct grids ;
    particle_struct particles ;

    get_grid_profile     ( &grids     , &input_tag ) ;
    get_particle_profile ( &particles , &input_tag ) ;

    get_grid_memory     ( &grids     , gpu.context ) ;
    get_particle_memory ( &particles , gpu.context ) ;

    cl_bool asynchronous = CL_FALSE ;
    unsigned int offset = 0 ;

    const cl_int write_event_count = 3 ;
    cl_event write_event [ write_event_count ] ;
    CL_CHECK ( clEnqueueWriteBuffer( gpu.queue[0], grids.cl_position    ,  asynchronous,    offset,  grids.cl_position_bytes    , grids.position    , 0, NULL, write_event   ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( gpu.queue[0], particles.cl_position,  asynchronous,    offset,  particles.cl_position_bytes, particles.position, 0, NULL, write_event+1 ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( gpu.queue[0], particles.cl_velocity,  asynchronous,    offset,  particles.cl_velocity_bytes, particles.velocity, 0, NULL, write_event+2 ) ) ;
    
    CL_CHECK( clWaitForEvents( write_event_count , write_event ) );
    for ( int i = 0 ; i < write_event_count ; i++ ){ CL_CHECK( clReleaseEvent( write_event [ i ] )   ) ;  }

    finish_queue ( &gpu );

    free_k ( hello_world_kernel ) ;
    
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}
