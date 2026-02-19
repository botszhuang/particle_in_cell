#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <cl_info.h>

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


    finish_queue ( &gpu );

    free_queue   ( &gpu );
    free_context ( &gpu ) ;


    return EXIT_SUCCESS;
}
