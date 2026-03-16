#include <platform.h>
#include <hello_world.h>

void initialize_the_platform ( platform_struct * g ) {

    print_all_platform_info () ;

    cl_kernel hello_kernel ;
    cl_command_queue hello_queue ;

    select_the_platform ( g ) ;
    get_device_ids ( g->platform , & g->devices , & g->device_N ) ;
    create_context ( g );
    
    create_queue_in_order ( & hello_queue , g->context, g->devices[0] ) ;
    
    init_program ( g );

    puts("### Running hello world kernel ......") ;
    create_hello_world_kernel ( &hello_kernel , g->program ) ;
    run_kernel_hello_world ( hello_kernel , hello_queue ) ;

    CL_CHECK( clFlush ( hello_queue ) );
    CL_CHECK( clFinish( hello_queue ) );

    free_kernel ( hello_kernel ) ;

    if ( hello_queue ) {
        CL_CHECK( clReleaseCommandQueue ( hello_queue ) ) ; 
    }

}

void free_platform_struct ( platform_struct * g ) {
    
    cl_int ret = 0 ;

    // program
    if ( g->program ){ 
        ret = clReleaseProgram(g->program) ; 
        CL_CHECK( ret ) ; 
    }
    
    // context
    if ( g->context  ){ 
        ret = clReleaseContext(g->context) ; 
        CL_CHECK( ret ) ; 
    }

    // devices
    if ( g->devices  ){ 
        for ( unsigned int i = 0 ; i < g->device_N ; i++ ) {
            if ( g->devices[i] ){ 
                ret = clReleaseDevice ( g->devices[i] ) ; 
                CL_CHECK( ret ) ; 
            }
        }
        free ( g->devices ) ; 
    }
    
}