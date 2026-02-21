#include <platform.h>

void create_context ( platform_struct * a ){

   cl_int ret = 0 ;

   a->context = clCreateContext( NULL, a->device_N , a->devices , NULL, NULL, &ret);
   CL_CHECK( ret ) ;

}
