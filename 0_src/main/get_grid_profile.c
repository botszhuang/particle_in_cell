#include <main_struct.h>

void get_grid_profile ( grid2_struct * g , input_tex_tag_struct * tag , platform_struct * gpu ) {

    puts("### Reading grid profiles ......") ;

    cl_int ret = 0 ;
    size_t N = 0 ;

    grid_dimension * grid_ptr ;
    
    N = read_2D( &grid_ptr , tag->grid_file ) ;
   
    g->grid_total_number = N ;
    g->X = grid_ptr ;

    size_t bytes = N * sizeof ( grid_ptr [0] ) ;

    // single buffer
    g->dev_X.bytes = bytes ;
    g->dev_X.host = g->X ;
    g->dev_X.buffer = clCreateBuffer ( gpu->context,  CL_MEM_READ_WRITE , bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    create_and_set_event_CL_COMPLETE ( &( g->dev_X.io    ), gpu ) ;
    create_and_set_event_CL_COMPLETE ( &( g->dev_X.print ), gpu ) ;
   

    // double buffer
    for ( unsigned int i = 0 ; i < 2 ; ++i ) {
        g->dev_E[ i ].bytes = bytes ;
        g->E[ i ] = malloc ( bytes ) ;
        g->dev_E[ i ].host = g->E[ i ] ;
        g->dev_E[ i ].buffer = clCreateBuffer    ( gpu->context,  CL_MEM_READ_WRITE , bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
        create_and_set_event_CL_COMPLETE ( &( g->dev_E[ i ].io    ), gpu ) ;
        create_and_set_event_CL_COMPLETE ( &( g->dev_E[ i ].print ), gpu ) ;    
    }
 

}
