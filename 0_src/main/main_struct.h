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
    cl_uint dim ;
    size_t * global_offset ;
    size_t * global_size ;
    size_t * local_size ;
} work_dim_struct ;

typedef struct {
    char * grid_file ;
    char * particle_position_file ;
    char * particle_velocity_file ;   
} input_tex_tag_struct ;

typedef struct {
    size_t bytes ;
    cl_mem buffer ;
    cl_event io ;
    cl_event print ;
    void * host ;
}buffer_struct;

typedef struct {
    unsigned int number ;
    particle_dimension * X [2] ;
    particle_dimension * V [2] ;
    particle_dimension * F [2] ;

    buffer_struct dev_X [2];
    buffer_struct dev_V [2];
    buffer_struct dev_F [2];
} particle_struct;

typedef struct {
    unsigned int grid_total_number ;

    grid_dimension * X  ;
    buffer_struct dev_X ;

    grid_dimension * E [2] ;
    buffer_struct dev_E [2] ;
} grid2_struct ;


typedef struct {
    cl_mem dev_X ;
    size_t dev_X_bytes ;
    grid_dimension * h_X ;
    unsigned int h_number ;
    cl_event X_io ;
} grid_struct;

typedef struct{
    cl_event io ;
    cl_event print ;
}io_event_struct ;
typedef struct {

    particle_dimension * h_particle_X ;
    particle_dimension * h_particle_V ;
    particle_dimension * h_particle_F ;
    unsigned int h_particle_number ;

    cl_mem dev_particle_X ;
    cl_mem dev_particle_V_half ;
    cl_mem dev_particle_F ;
    size_t dev_particle_X_bytes; 
    size_t dev_particle_V_half_bytes; 
    size_t dev_particle_F_bytes; 

    cl_mem dev_grid_E ;
    size_t dev_grid_E_bytes ;
    grid_dimension * h_grid_E ;
    unsigned int h_grid_number ;

    leap_frog_kernel_struct leap_frog ;
    cl_kernel force_kernel  ;
    cl_kernel grid_E_kernel ;

    io_event_struct X ;
    io_event_struct V ;
    io_event_struct F ;
    io_event_struct grid_E ;

    unsigned int i ;
} sync_cl_Struct ;

typedef struct {
    char fname [128] ;
    dimension_2D_struct * p ;
    unsigned int number ;
    cl_event * waitList ;
    cl_event print ;
}myCall_struct;

#define ASYNCHRONOUS CL_FALSE 
extern pthread_mutex_t printLocker ;


void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile );
void get_grid_profile     ( grid2_struct    * g , input_tex_tag_struct * tag , platform_struct * gpu ) ;
void get_particle_profile ( particle_struct * p , input_tex_tag_struct * tag , platform_struct * gpu ) ;

void create_and_set_event_CL_COMPLETE ( cl_event * e , platform_struct * gpu ) ;

void init_leapfrog_kernels_and_args( leap_frog_kernel_struct * kernels , particle_struct * p , grid2_struct * grid , platform_struct * gpu , myfloat * dt );
void init_force_kernels_and_args( cl_kernel * kernel , particle_struct * p , grid2_struct *grid , platform_struct * gpu );

void copy_data_to_dev ( cl_command_queue io_queue , particle_struct * p , grid2_struct *grid , cl_event * dummy_event ) ;
void init_V_half ( cl_command_queue dev_queue , particle_struct * p , grid2_struct * grid , 
                   cl_kernel * force_kernels , leap_frog_kernel_struct * leap_frog_kernel ,
                   work_dim_struct * w , cl_event * dummy_event , double * dt  ) ;

void print_2D ( const unsigned int i ,
                const unsigned int N ,
                particle_dimension * P ) ;

void freeEventArray( const unsigned int N , cl_event * eventArray ) ;

typedef struct {
    cl_command_queue dev_queue ;
    cl_command_queue io_queue ;
    work_dim_struct  w ; 
    unsigned int LOOP_N ;
    platform_struct * gpu ;
    cl_event  * func_event ;
    cl_event * waitlist ;
    unsigned int waitlist_size ;
} leapfrog_step1_struc;

//------------ callback function --------------//
void CL_CALLBACK on_io_complete(cl_event event, cl_int status, void* user_data) ;

void io_and_print( buffer_struct * Xcurrent ,
                   cl_event * LF_X , 
                   char * str0 ,
                   const unsigned int i , 
                   const unsigned int LOOP_N ,
                   const unsigned int P_number ,
                   cl_command_queue io_queue ,
                   cl_context context );
