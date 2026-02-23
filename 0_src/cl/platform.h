#pragma once
#include <stdbool.h>
#include <cl_version.h>
#include <cl_erro_code.h>

typedef struct{

  cl_platform_id platform ;

  unsigned int   device_N ;
  cl_device_id * devices ;

  cl_context context ;
  cl_command_queue  * queue ;

  cl_program program ;

}platform_struct ;

typedef struct{
    cl_event * array ;
    const unsigned int count ; 
} cl_event_struct ;

void print_all_platform_info () ;
void print_a_platform_info ( const cl_platform_id id ) ;
void select_the_platform  ( platform_struct * g ) ;
void free_platform_struct ( platform_struct * g ) ;

void get_device_ids ( cl_platform_id id , cl_device_id ** d , unsigned int * N );
unsigned int list_devices( cl_platform_id p , bool print ) ;
void create_context ( platform_struct * g );
void create_queue ( platform_struct * g );
void finish_queue ( platform_struct * g );
void init_program ( platform_struct * g ) ;

// event
cl_event_struct initEventStruct( const unsigned int n) ;
#define waitForEvents(ev) { CL_CHECK( clWaitForEvents( ev.count , ev.array ) ) ; }
#define releaseEventArray(ev) {\
    for ( unsigned int i = 0 ; i < ev.count ; i++ ) {\
        CL_CHECK( clReleaseEvent( ev.array[i] ) ) ;\
    }\
}

// kernel
#define free_kernel(x) { if ( x ){ clReleaseKernel ( x ) ; } }
