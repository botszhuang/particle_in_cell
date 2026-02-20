
#include <cl_platform_struct.h>
#include <cl_kernel_list.h>
#include <input/input.h>
#include <particle_and_grid/grid.h>
#include <particle_and_grid/particle.h>

typedef struct{
    particle_struct particles ;    
    grid_struct grids ;
} data_struct ;

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


    key_value_pair_struct * keys = set_key( ) ;
    read_parameters( keys , inputFile ) ;
    print_parameters( keys ) ;

    data_struct data ;

    data.grids.number = read_2D( & ( data.grids.position ) , keys[0].value ) ;
    print_2D_list( data.grids.position , data.grids.number ) ;

    data.particles.number = read_2D( & ( data.particles.position ) , keys[1].value ) ;
    print_2D_list( data.particles.position , data.particles.number ) ;

    data.particles.number = read_2D( & ( data.particles.velocity ) , keys[2].value ) ;
    print_2D_list( data.particles.velocity , data.particles.number ) ;


    finish_queue ( &gpu );

    free_k ( hello_world_kernel ) ;
    
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}
