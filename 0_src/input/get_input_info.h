#pragma once

#include <stdlib.h>

typedef struct {

    char * grid_file ;
    char * particle_position_file ;
    char * particle_velocity_file ;
    
} input_tex_tag_struct ;

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ) ;
