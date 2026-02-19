#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <cl_platform_struct.h>
#include <cl_kernel_list.h>

int main(){

  
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


    finish_queue ( &gpu );

    free_k ( hello_world_kernel ) ;
    
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}
