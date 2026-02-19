#include <cl_platform_struct.h>

void create_context ( cl_platform_struct * a ){

   cl_int ret = 0 ;

   a->context = clCreateContext( NULL, a->device_N , a->devices , NULL, NULL, &ret);

}
