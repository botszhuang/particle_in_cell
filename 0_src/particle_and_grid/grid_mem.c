#include <cl_version.h>
#include <cl_erro_code.h>
#include <grid_struct.h>
#include <get_input_info.h>

void get_grid_memory ( grid_struct * grids , cl_context context ) {
    cl_int ret = 0 ;  
    grids->cl_position_bytes = grids->number     * sizeof( grid_dimension     ) ;
    grids->cl_position = clCreateBuffer( context , CL_MEM_READ_WRITE , grids->cl_position_bytes , NULL , &ret ) ; CL_CHECK( ret ) ; 
}
void free_grid_memory ( grid_struct * grids  ) {
    CL_CHECK( clReleaseMemObject( grids->cl_position ) ) ;
}
void get_grid_profile ( grid_struct * grids , input_tex_tag_struct * input_tag ) {
    grids->number = read_2D( & ( grids->position ) , input_tag->grid_file ) ;
    //print_2D_list( grids->position , grids->number ) ;
}
void write_grid_to_device ( grid_struct * grids , cl_command_queue queue , cl_event * write_event ) {
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, grids->cl_position    ,  asynchronous,    offset,  grids->cl_position_bytes    , grids->position    , 0, NULL, write_event   ) ) ;
}


