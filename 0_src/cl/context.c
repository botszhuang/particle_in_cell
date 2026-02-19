#include <cl_info.h>

void create_context ( cl_platform_struct * a ){

   cl_uint ret = 0 ;

   a->context = clCreateContext( NULL, a->device_N , a->devices , NULL, NULL, &ret);

}
void free_context ( cl_platform_struct * g ) {
    if( g->context ){
        clReleaseContext(g->context) ;
    }
}

