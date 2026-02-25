#pragma once 

#include <stdlib.h>
#include <platform.h>
#include <get_input_info.h>
#include <cl_kernel_list.h>

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
    cl_mem dt ;
    size_t X_bytes ;
    size_t V_bytes ;
    size_t F_bytes ;
    size_t mass_bytes ;
} particle_cl_mem_struct ;

typedef struct {
    cl_mem X ;
    size_t X_bytes ;
} grid_cl_mem_struct;

typedef struct{
    size_t number ;
    particle_dimension * X ;
    particle_dimension * V ;
    particle_dimension * F ;
} particle_struct ;

typedef struct{
    size_t number;
    grid_dimension * X ;
} grid_struct ;

typedef struct {
   myfloat current_T ;
   myfloat dt ;
   particle_struct particle ;
   particle_cl_mem_struct particle_cl_mem ;
   grid_struct grid ;
   grid_cl_mem_struct grid_cl_mem ;
}data_struct;

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile );
void get_data_profile     ( data_struct * data , input_tex_tag_struct * tag );
void create_dev_memory    ( data_struct * data , platform_struct * g ) ;
void free_dev_memory      ( data_struct * data ) ;
void write_data_to_device ( data_struct *data , cl_command_queue queue , cl_event * write_event ) ;
void read_data_to_host ( data_struct * data , cl_command_queue queue , cl_event_struct wait ) ;
