
#include <cl_platform_struct.h>

void free_platform_struct ( cl_platform_struct * g ) {
    
    if ( g->program ){ clReleaseProgram(g->program) ; }
    
    if ( g->queue ) {
        for ( unsigned int i = 0 ; i < g->device_N ; i++ ) {
            if ( g->queue[i] ){ clReleaseCommandQueue ( g->queue[i] ) ; }
        }
        free ( g->queue ) ;
    }

    if ( g->context  ){ clReleaseContext(g->context) ; }

    if ( g->devices  ){ 
        for ( unsigned int i = 0 ; i < g->device_N ; i++ ) {
            if ( g->devices[i] ){ clReleaseDevice ( g->devices[i] ) ; }
        }
        free ( g->devices ) ; 
    }
    
}