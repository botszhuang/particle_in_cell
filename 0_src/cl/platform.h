#pragma once
#include <stdbool.h>
#include <cl_version.h>
#include <cl_erro_code.h>

typedef struct{

  cl_platform_id platform ;

  unsigned int   device_N ;
  cl_device_id * devices ;

  cl_context context ;

  cl_program program ;

}platform_struct ;

typedef struct{
    cl_event * array ;
    unsigned int count ; 
} cl_event_struct ;

void print_all_platform_info () ;
void print_a_platform_info ( const cl_platform_id id ) ;
void select_the_platform  ( platform_struct * g ) ;
void initialize_the_platform ( platform_struct * g ) ;
void free_platform_struct ( platform_struct * g ) ;

void get_device_ids ( cl_platform_id id , cl_device_id ** d , unsigned int * N );
unsigned int list_devices( cl_platform_id p , bool print ) ;
void create_context ( platform_struct * g );
void init_program ( platform_struct * g ) ;

void create_queue_in_order     ( cl_command_queue * q , cl_context context, cl_device_id device ) ;
void create_queue_out_of_order ( cl_command_queue * q , cl_context context, cl_device_id device ) ;

// kernel
#define free_kernel(x) { if ( x ){ clReleaseKernel ( x ) ; } }
