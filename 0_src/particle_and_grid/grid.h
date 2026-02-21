#pragma once
#include <cl_version.h>
#include <dimension.h>
#include <get_input_info.h>
#include <grid_struct.h>

void get_grid_profile ( grid_struct * grids ,  input_tex_tag_struct * input_tag ) ;
void get_grid_memory ( grid_struct * grids , cl_context context ) ;

