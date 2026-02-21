#pragma once

#include <stdlib.h>
#include <dimension.h>

typedef struct {

    char * grid_file ;
    char * particle_position_file ;
    char * particle_velocity_file ;
    
} input_tex_tag_struct ;

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ) ;

void init_read_input_tex( char * inputFile );
void close_read_input_tex() ;
char * read_input_tex( char * key ) ;

int read_2D ( dimension_2D_struct ** out , char * inputFile ) ;
void print_2D_list( dimension_2D_struct * list , size_t list_size ) ;
