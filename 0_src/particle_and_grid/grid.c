#include <cl_erro_code.h>
#include <read_input_tex.h>
#include <get_input_info.h>
#include <grid.h>
#include <particle.h>


void get_grid_profile ( grid_struct * grids ,  input_tex_tag_struct * input_tag ) {

    grids->number = read_2D( & ( grids->position ) , input_tag->grid_file ) ;

    print_2D_list( grids->position , grids->number ) ;
}

void get_grid_memory ( grid_struct * grids , cl_context context ) {

    grids->cl_position_bytes = grids->number     * sizeof( grid_dimension     ) ;
    
    printf("grid position bytes: %i\n" , grids->cl_position_bytes ) ;
    print_2D_list( grids->position  , grids->number ) ;

    cl_int ret = 0 ;    
    grids->cl_position = clCreateBuffer( context , CL_MEM_READ_WRITE , grids->cl_position_bytes , NULL , &ret ) ; CL_CHECK( ret ) ; 

}

