#pragma once 

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#include <cl_erro_code.h>
#include <platform.h>
#include <get_input_info.h>
#include <cl_kernel_list.h>


typedef struct {
    cl_mem X ;
    size_t X_bytes ;
} grid_cl_mem_struct;
typedef struct{
    unsigned int number;
    grid_dimension * X ;
} grid_struct ;

typedef struct {
    char * grid_file ;
    char * particle_position_file ;
    char * particle_velocity_file ;   
} input_tex_tag_struct ;
typedef struct {
    cl_mem X ;
    cl_mem V ;
    cl_mem F ;
    cl_mem mass ;
    size_t bytesX, bytesV ,bytesF ;
} particle_cl_mem_struct ;
typedef struct{
    particle_dimension * X ;
    particle_dimension * V ;
    particle_dimension * F ;
    unsigned int number ;
} particle_struct ;
typedef struct{
    cl_event io ;
    cl_event print ;
}io_event_struct ;
typedef struct {
    particle_struct p ;
    particle_cl_mem_struct pCL ;
    leap_frog_kernel_struct leap_frog ;
    force_kernel_struct force  ;
    io_event_struct X ;
    io_event_struct V ;
    io_event_struct F ;
    unsigned int i ;
} sync_cl_Struct ;



typedef struct {
    char str0[32];
    dimension_2D_struct * p ;
    unsigned int number ;
    cl_event * waitList ;
    float i ; 
    cl_event print ;
}myCall_struct;

#define ASYNCHRONOUS CL_FALSE 
extern pthread_mutex_t printLocker ;


void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile );
void get_grid_profile ( grid_struct * g , input_tex_tag_struct * tag ) ;
void get_particle_profile ( particle_struct * p , input_tex_tag_struct * tag ) ;
void create_grid_dev_memory ( grid_cl_mem_struct * gCL , const grid_struct g , platform_struct gpu ) ;
void free_grid_dev_memory ( grid_cl_mem_struct * gCL );
void free_particle_dev_memory ( particle_cl_mem_struct * pCL );
void create_particle_dev_memory( sync_cl_Struct * sync , platform_struct * gpu ) ;
void init_double_buffer_events ( sync_cl_Struct * sync , platform_struct * gpu ) ;
void init_double_buffer_kernels_and_args( sync_cl_Struct *sync ,  platform_struct * gpu , myfloat * dt );
 
void print_2D ( const unsigned int i ,
                const unsigned int N ,
                particle_dimension * P ) ;

void freeEventArray( const unsigned int N , cl_event * eventArray ) ;


//------------ callback function --------------//
//void myCall( dimension_2D_struct * p , const unsigned int pN, const char * str0 , const float i ) ;
//void CL_CALLBACK on_io_complete(cl_event event, cl_int status, void* user_data) ;
void run_io_queue ( cl_command_queue * io_queue  , cl_event * funcEvent , platform_struct * gpu , io_event_struct * X , cl_mem pCLX , const size_t pCL_bytesX , particle_dimension * pX , const size_t p_number , const unsigned int LOOP_N , const unsigned int i , const float iplus , char * str0 ) ;

