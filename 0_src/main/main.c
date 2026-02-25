#include <main_struct.h>

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
    printf("-----------------------\n"); 
    printf("Hello world!\n") ;

    platform_struct gpu ;
    input_tex_tag_struct input_tag ;
    
    data_struct data;
    
    leap_frog_kernel_struct leap_frog ;
    force_kernel_struct force ;
    cl_command_queue io_queue  ;
    cl_command_queue dev_queue  ;

    cl_uint work_dim = 1 ;
    size_t global_work_offset[1] = {0} ;
    size_t global_work_size[1] = {1} ;
    size_t local_work_size[1] = {1} ;

    // --------------------------------------------------------------------------

    initialize_the_platform ( & gpu ) ;
    get_input_tex ( &input_tag , inputFile ) ;
    get_data_profile ( &data , &input_tag ) ;

    // --------------------------------------------------------------------------
    // events
    const cl_int write_grid_count     = 1 ;
    const cl_int write_particle_count = 2 ;
    cl_event_struct write_events = initEventStruct ( write_grid_count + write_particle_count ) ;
    cl_event_struct leap_frog_V_half_events = initEventStruct( 1 ) ;
    cl_event_struct leap_frog_X_events = initEventStruct( 1 ) ;
    cl_event_struct force_events = initEventStruct ( 1 ) ;

    // --------------------------------------------------------------------------
    puts("### Allocating memory on the device ......") ;
    create_dev_memory ( &data , &gpu ) ;

    puts("### Creating kernels ......") ;
    create_leap_frog_kernel ( &leap_frog, gpu,  work_dim, global_work_offset, global_work_size, local_work_size ) ; 
    create_force_G_kernel ( &force, gpu,  work_dim, global_work_offset, global_work_size, local_work_size ) ; 

    // set kernels' args
    puts("### Setting the arguments for kernels ......") ;
    set_leap_frog_kernel_args ( & leap_frog , 
                                &( data.particle_cl_mem.X ), &( data.particle_cl_mem.V ), &( data.particle_cl_mem.F ) ,
                                &( data.particle.number   ), &( data.dt )  ) ;
    set_force_G_kernel_args ( & force , &( data.particle_cl_mem.F ), &( data.particle.number ) ) ;

    // --------------------------------------------------------------------------
 
    create_queue_in_order     ( &io_queue  , gpu.context , gpu.devices[0] ) ;
    create_queue_out_of_order ( &dev_queue , gpu.context , gpu.devices[0] ) ;

    puts("### Writing grid and particle data to the device ......") ;
    write_data_to_device ( &data  , io_queue , write_events.array ) ;

    puts("### Running kernels ......") ;
    
    // leap frog step 0 : init V_half
    run_init_leap_frog_V_half_kernel ( & leap_frog , data.dt , dev_queue , write_events , leap_frog_V_half_events.array ) ;
    releaseEventArray( write_events ) ;

    for ( unsigned int i = 0 ; i < 5 ; i++ ) {
        // leap frog step 1 :
        run_leap_frog_X_kernel ( & leap_frog , dev_queue , leap_frog_V_half_events , leap_frog_X_events.array ) ;
        releaseEventArray( leap_frog_V_half_events ) ;
        
        // leap frog step 2 :   a = A(x)  compute A
        run_force_G_kernel ( & force , dev_queue , leap_frog_X_events , force_events.array ) ;

        // leap frog step 3 :
        run_leap_frog_V_half_kernel ( & leap_frog , dev_queue , force_events , leap_frog_V_half_events.array ) ;
 
        releaseEventArray( leap_frog_X_events ) ;       
        releaseEventArray( force_events ) ;

        //if ( ( i+1 )% 5 ) { continue; }
        read_data_to_host ( & data , io_queue , leap_frog_V_half_events ) ;

    }
    puts("### Waiting for the kernels to finish ......") ;
    waitForEvents( leap_frog_V_half_events );
    releaseEventArray( leap_frog_V_half_events ) ;

    // --------------------------------------------------------------------------
 
    
    free_leap_frog_kernel ( & leap_frog ) ;
    free_force_G_kernel ( & force ) ;

    free_dev_memory ( & data ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

