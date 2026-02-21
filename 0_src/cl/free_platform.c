
#include <cl_platform_struct.h>

void free_platform_struct ( cl_platform_struct * g ) {
    
    cl_int ret = 0 ;

    // program
    if ( g->program ){ 
        ret = clReleaseProgram(g->program) ; 
        CL_CHECK( ret ) ; 
    }
    
    // queue
    if ( g->queue ) {
        for ( unsigned int i = 0 ; i < g->device_N ; i++ ) {
            if ( g->queue[i] ){ 
                ret = clReleaseCommandQueue ( g->queue[i] ) ; 
                CL_CHECK( ret ) ; 
            }
        }
        free ( g->queue ) ;
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