#pragma once 

#include <stdlib.h>
#include <cl_erro_code.h>
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

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile );
void get_data_profile ( grid_struct * g , particle_struct * p , input_tex_tag_struct * tag ) ;
void create_dev_memory ( grid_cl_mem_struct * gCL , particle_cl_mem_struct * pCL , const grid_struct g , const particle_struct p , platform_struct gpu ) ;
void free_dev_memory ( grid_cl_mem_struct * gCL , particle_cl_mem_struct * pCL ) ;
