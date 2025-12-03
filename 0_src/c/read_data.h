#ifndef READ_DATA_H
#define READ_DATA_H

#include <cell_type.h>
#include <test_particle_type.h>

void read_points ( test_particle_profile_struct * p ) ;

void read_node ( cell_profile_struct * c ) ;
void read_cell ( cell_profile_struct * c ) ;

#endif