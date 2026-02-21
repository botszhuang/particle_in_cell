#include <cl_erro_code.h>
#include <read_input_tex.h>
#include <get_input_info.h>
#include <grid.h>
#include <particle.h>


void get_grid_profile ( grid_struct * grids ,  input_tex_tag_struct * input_tag ) {

    grids->number = read_2D( & ( grids->position ) , input_tag->grid_file ) ;

    print_2D_list( grids->position , grids->number ) ;
}


