
#include <cl_platform_struct.h>
#include <cl_kernel_list.h>
#include <grid.h>
#include <particle.h>
#include <get_input_info.h>

void get_memory ( grid_struct * grids , particle_struct * particles , cl_context context ) ;
void free_memory ( grid_struct * grids , particle_struct * particles ) ;
void write_grid_and_particle_to_device ( grid_struct * grids , particle_struct * particles , cl_command_queue queue , cl_event * write_event ) ;

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
  
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

    cl_kernel hello_world_kernel ;
    create_hello_world_kernel ( &hello_world_kernel , gpu.program ) ;
    run_kernel_hello_world ( hello_world_kernel , gpu.queue[0] ) ;

    



  /*  grid_struct grids ;
    particle_struct particles ;

    get_grid_profile     ( &grids     , &input_tag ) ;
    get_particle_profile ( &particles , &input_tag ) ;
    get_memory ( &grids , &particles , gpu.context ) ;

    const cl_int write_event_count = 3 ;
    cl_event write_event [ write_event_count ] ;

    write_grid_and_particle_to_device ( &grids , &particles , gpu.queue[0] , write_event ) ;
    
    CL_CHECK( clWaitForEvents( write_event_count , write_event ) );
    for ( int i = 0 ; i < write_event_count ; i++ ){ CL_CHECK( clReleaseEvent( write_event [ i ] )   ) ;  }
*/
    finish_queue ( &gpu );

    free_k ( hello_world_kernel ) ;
//    free_memory ( &grids , &particles ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

void get_memory ( grid_struct * grids , particle_struct * particles , cl_context context ) {

    cl_int ret = 0 ;  

    grids->cl_position_bytes = grids->number     * sizeof( grid_dimension     ) ;
    grids->cl_position = clCreateBuffer( context , CL_MEM_READ_WRITE , grids->cl_position_bytes , NULL , &ret ) ; CL_CHECK( ret ) ; 


    particles->cl_position_bytes     = particles->number * sizeof( particle_dimension ) ;
    particles->cl_velocity_bytes     = particles->number * sizeof( particle_dimension ) ;
    particles->cl_acceleration_bytes = particles->number * sizeof( particle_dimension ) ;

    particles->cl_position     = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_position_bytes     , NULL , &ret ) ; CL_CHECK( ret ) ;
    particles->cl_velocity     = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_velocity_bytes     , NULL , &ret ) ; CL_CHECK( ret ) ;
    particles->cl_acceleration = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_acceleration_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

}
void free_memory ( grid_struct * grids , particle_struct * particles ) {
    CL_CHECK( clReleaseMemObject( grids->cl_position ) ) ;
    CL_CHECK( clReleaseMemObject( particles->cl_position ) ) ;
    CL_CHECK( clReleaseMemObject( particles->cl_velocity ) ) ;
    CL_CHECK( clReleaseMemObject( particles->cl_acceleration ) ) ;
}
void write_grid_and_particle_to_device ( grid_struct * grids , particle_struct * particles , cl_command_queue queue , cl_event * write_event ) {
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, grids->cl_position    ,  asynchronous,    offset,  grids->cl_position_bytes    , grids->position    , 0, NULL, write_event   ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, particles->cl_position,  asynchronous,    offset,  particles->cl_position_bytes, particles->position, 0, NULL, write_event+1 ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, particles->cl_velocity,  asynchronous,    offset,  particles->cl_velocity_bytes, particles->velocity, 0, NULL, write_event+2 ) ) ;
}