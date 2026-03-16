#include <main_struct.h>

void create_and_set_event_CL_COMPLETE ( cl_event * e , platform_struct * gpu ){
    cl_int ret = 0 ;
    *e = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ; 
    CL_CHECK( clSetUserEventStatus( *e , CL_COMPLETE) ) ;
}

