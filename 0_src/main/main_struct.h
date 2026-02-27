#pragma once 

#include <stdlib.h>
#include <math.h>

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
    cl_event ioGPU ;
}io_event_struct ;
typedef struct {
    particle_struct p ;
    particle_cl_mem_struct pCL ;
    leap_frog_kernel_struct leap_frog ;
    force_kernel_struct force  ;
    cl_event LF_V ;
    cl_event LF_X ;
    cl_event GET_FORCE ;
    cl_event ioX ;
    cl_event ioV ;
    cl_event ioF ;
    cl_event printX ;
    cl_event printV ;
    cl_event printF ;
    unsigned int i ;
} sync_cl_Struct ;



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
