
#include <cl_platform_struct.h>
#include <cl_kernel_list.h>
#include <grid.h>
#include <particle.h>
#include <get_input_info.h>

#define printLine()   { printf("-----------------------\n"); }

void release_event_array ( cl_event * event_array , unsigned int event_count ) ;

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
    printLine() ; 
    printf("Hello world!\n") ;

    // read input.tex
    input_tex_tag_struct input_tag ;
    get_input_tex ( &input_tag , inputFile ) ;

    // initialize the platform
    print_all_platform_info () ;

    cl_platform_struct gpu ;

    select_the_platform ( &gpu ) ;

    get_device_ids ( gpu.platform , & gpu.devices , & gpu.device_N ) ;

    create_context ( &gpu );
    create_queue   ( &gpu );

    init_program ( &gpu );

    puts("### Running hello world kernel ......") ;
    cl_kernel hello_world_kernel ;
    create_hello_world_kernel ( &hello_world_kernel , gpu.program ) ;
    run_kernel_hello_world ( hello_world_kernel , gpu.queue[0] ) ;
  
    
    grid_struct grids ;
    particle_struct particles ;

    get_grid_profile ( &grids , &input_tag ) ;
    get_particle_profile ( &particles , &input_tag ) ;

    get_grid_memory ( &grids , gpu.context ) ;
    get_particle_memory ( &particles , gpu.context ) ;


    const cl_int write_grid_count     = 1 ;
    const cl_int write_particle_count = 2 ;
    cl_event write_grid_event     [ write_grid_count     ] ;
    cl_event write_particle_event [ write_particle_count ] ;

    write_grid_to_device     ( &grids     , gpu.queue[0] , write_grid_event ) ;
    write_particle_to_device ( &particles , gpu.queue[0] , write_particle_event ) ;

    CL_CHECK( clWaitForEvents( write_grid_count, write_grid_event ) );
    CL_CHECK( clWaitForEvents( write_particle_count, write_particle_event ) );

    release_event_array( write_grid_event     , write_grid_count     ) ;
    release_event_array( write_particle_event , write_particle_count ) ;


    finish_queue ( &gpu );

    free_k ( hello_world_kernel ) ;
    free_grid_memory ( &grids ) ;
    free_particle_memory ( &particles ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

void release_event_array ( cl_event * event_array , unsigned int event_count ) {
    for ( unsigned int i = 0 ; i < event_count ; i++ ) {
        CL_CHECK( clReleaseEvent( event_array[i] ) ) ;
    }
}
