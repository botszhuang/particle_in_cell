#ifndef CELL_RUN_H
#define CELL_RUN_H

#include <cell_type.h>

void print_nodes_in_cell ( cell_profile_struct * c ) ;
void print_cells         ( cell_profile_struct * c ) ;

void cl_cell_mem_init  ( cell_profile_struct * c , cl_gpu_profile_struct * g ) ;
void free_cell_profile ( cell_profile_struct * c ) ;
#endif