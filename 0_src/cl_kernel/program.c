#include <stdlib.h>

#include <cl_info.h>
#include "hello_world_cl.h"

void init_program( cl_platform_struct * g ){
  
    const char* sources[] = { (char*)hello_world_cl };
    size_t sizes[] = { hello_world_cl_len };

    unsigned int count = sizeof ( sources ) / sizeof ( sources[0] ) ;

    cl_uint err = 0 ;

    g->program = clCreateProgramWithSource( g->context , count , sources, sizes, &err);
    err = clBuildProgram( g->program , g->device_N , g->devices , NULL , NULL, NULL) ;

}
