#pragma once
#include <stdbool.h>
#include <cl_version.h>

typedef struct{

  cl_platform_id platform ;

  unsigned int   device_N ;
  cl_device_id * devices ;

  cl_context context ;
  cl_command_queue  * queue ;

  cl_program program ;

}cl_platform_struct ;

void print_all_platform_info () ;
void print_a_platform_info ( const cl_platform_id id ) ;
void select_the_platform  ( cl_platform_struct * g ) ;
void free_platform_struct ( cl_platform_struct * g ) ;

void get_device_ids ( cl_platform_id platform_id , cl_device_id ** dev , unsigned int * dev_n ) ;
unsigned int list_devices( cl_platform_id platform_id , bool print ) ;

void create_context ( cl_platform_struct * g );

void create_queue ( cl_platform_struct * g );
void finish_queue ( cl_platform_struct * g );

void init_program ( cl_platform_struct * g ) ;
