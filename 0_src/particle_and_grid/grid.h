#pragma once

#include <grid_struct.h>
#include <get_input_info.h>

void get_grid_memory ( grid_struct * grids , cl_context context ) ;
void free_grid_memory ( grid_struct * grids  ) ;
void get_grid_profile ( grid_struct * grids , input_tex_tag_struct * input_tag ) ;
void write_grid_to_device ( grid_struct * grids , cl_command_queue queue , cl_event * write_event ) ;
