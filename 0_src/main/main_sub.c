#include <main_struct.h>

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ){
    printf("### Reading %s ......\n" , inputFile ) ;
    init_read_input_tex( inputFile ) ;
    input_tag->grid_file = read_input_tex( "grid_file" ) ;
    input_tag->particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag->particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;
}

void get_data_profile ( grid_struct * g , particle_struct * p , input_tex_tag_struct * tag ) {

    puts("### Reading grid and particle profiles ......") ;


    // grid
    g->number = read_2D( & ( g->X ) , tag->grid_file ) ; 
    
    // particle
    p->number = read_2D( & ( p->X ) , tag->particle_position_file ) ;
    p->number = read_2D( & ( p->V ) , tag->particle_velocity_file ) ;

    p->F = calloc (  p->number , sizeof ( p->F[0] ) ) ;
    
    //print_2D_list( grid->position      , grid->number      ) ;
    //print_2D_list( particles->position , particles->number ) ;
    //print_2D_list( particles->velocity , particles->number ) ;

    if ( p->X == NULL || p->V == NULL || p->F == NULL) {
        fprintf(stderr, "Host memory allocation failed!\n");
        exit (1) ;
    }
}
void create_dev_memory ( grid_cl_mem_struct * gCL , 
                         particle_cl_mem_struct * pCL ,
                         const grid_struct g , 
                         const particle_struct p ,
                         platform_struct gpu ) {

    cl_int ret = 0 ;  
    
    // grid
    gCL->X_bytes = g.number * sizeof( grid_dimension ) ;
    gCL->X = clCreateBuffer( gpu.context , CL_MEM_READ_WRITE , gCL->X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

    // particle
    pCL->X_bytes = p.number * sizeof( p.X[0] ) ;
    pCL->V_bytes = p.number * sizeof( p.V[0] ) ;
    pCL->F_bytes = p.number * sizeof( p.F[0] ) ;
    
    pCL->X = clCreateBuffer( gpu.context , CL_MEM_READ_WRITE , pCL->X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    pCL->V = clCreateBuffer( gpu.context , CL_MEM_READ_WRITE , pCL->V_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    pCL->F = clCreateBuffer( gpu.context , CL_MEM_READ_WRITE , pCL->F_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
   
 }
void free_dev_memory ( grid_cl_mem_struct * gCL , particle_cl_mem_struct * pCL ) {
    
    // grid
    CL_CHECK( clReleaseMemObject( gCL->X ) ) ;
    
    // particle
    CL_CHECK( clReleaseMemObject( pCL->X ) ) ;
    CL_CHECK( clReleaseMemObject( pCL->V ) ) ;
    CL_CHECK( clReleaseMemObject( pCL->F ) ) ;
}
