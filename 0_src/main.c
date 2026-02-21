
#include <cl_platform_struct.h>
#include <cl_kernel_list.h>
#include <input.h>
#include <grid.h>
#include <particle.h>
#include <input_tex_tag_struct.h>

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
  
    printf("Hello world!\n") ;

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

    init_read_input_tex( inputFile ) ;
    input_tag.grid_file = read_input_tex( "grid_file" ) ;
    input_tag.particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag.particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;

    grid_struct grids ;
    particle_struct particles ;

    grids.number     = read_2D( & ( grids.position )     , input_tag.grid_file              ) ;
    particles.number = read_2D( & ( particles.position ) , input_tag.particle_position_file ) ;
    particles.number = read_2D( & ( particles.velocity ) , input_tag.particle_velocity_file ) ;

    print_2D_list( grids.position     , grids.number     ) ;
    print_2D_list( particles.position , particles.number ) ;
    print_2D_list( particles.velocity , particles.number ) ;

    

    finish_queue ( &gpu );

    free_k ( hello_world_kernel ) ;
    
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}
