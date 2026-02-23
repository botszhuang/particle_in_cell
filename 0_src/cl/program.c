#include <stdlib.h>

#include <platform.h>

#include "hello_world_cl.h"
#include "leap_frog_cl.h"
#include "dimension_h.h"
void init_program( platform_struct * g ){
  
    const char* sources[] = { (char*)dimension_h, 
                              (char*)hello_world_cl, 
                              (char*)leap_frog_cl };
    const size_t sizes[] = { dimension_h_len, 
                             hello_world_cl_len, 
                             leap_frog_cl_len } ;

    unsigned int count = sizeof ( sources ) / sizeof ( sources[0] ) ;

    const char * options = NULL;
    
    cl_int err = 0 ;

    g->program = clCreateProgramWithSource( g->context , count , sources, sizes, &err);
    err = clBuildProgram( g->program , g->device_N , g->devices , options , NULL, NULL) ;
    if ( err != CL_SUCCESS ) {
        printf("Error building program: %d\n", err);
        for ( unsigned int i = 0 ; i < g->device_N ; i++ ) {
            print_cl_build_log( g->program , g->devices [ i ] ) ;
        }
        exit(EXIT_FAILURE);
    }

}

