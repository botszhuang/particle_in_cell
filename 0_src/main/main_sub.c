#include <main_struct.h>

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ){
    printf("### Reading %s ......\n" , inputFile ) ;
    init_read_input_tex( inputFile ) ;
    input_tag->grid_file = read_input_tex( "grid_file" ) ;
    input_tag->particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag->particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;
}
#define p   data->particle
#define pCL data->particle_cl_mem
#define g   data->grid
#define gCL data->grid_cl_mem
void get_data_profile ( data_struct * data , input_tex_tag_struct * tag ) {

    puts("### Reading grid and particle profiles ......") ;

    data->current_T = 0 ;
    data->dt = 0.1 ;

    // grid
    g.number = read_2D( & ( g.X ) , tag->grid_file ) ; 
    
    // particle
    p.number = read_2D( & ( p.X ) , tag->particle_position_file ) ;
    p.number = read_2D( & ( p.V ) , tag->particle_velocity_file ) ;

    p.F = calloc (  p.number , sizeof ( p.F[0] ) ) ;
    
    //print_2D_list( grid->position      , grid->number      ) ;
    //print_2D_list( particles->position , particles->number ) ;
    //print_2D_list( particles->velocity , particles->number ) ;

    if ( p.X == NULL || p.V == NULL || p.F == NULL) {
        fprintf(stderr, "Host memory allocation failed!\n");
        exit (1) ;
    }
}
void create_dev_memory ( data_struct * data , platform_struct * gpu ) {

    cl_int ret = 0 ;  
    
    // grid
    gCL.X_bytes = g.number * sizeof( grid_dimension ) ;
    gCL.X = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , gCL.X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

    // particle
    pCL.X_bytes = p.number * sizeof( p.X[0] ) ;
    pCL.V_bytes = p.number * sizeof( p.V[0] ) ;
    pCL.F_bytes = p.number * sizeof( p.F[0] ) ;
    
    pCL.X = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , pCL.X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    pCL.V = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , pCL.V_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    pCL.F = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , pCL.F_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
   
 }
void free_dev_memory ( data_struct * data ) {
    
    // grid
    CL_CHECK( clReleaseMemObject( gCL.X ) ) ;
    
    // particle
    CL_CHECK( clReleaseMemObject( pCL.X ) ) ;
    CL_CHECK( clReleaseMemObject( pCL.V ) ) ;
    CL_CHECK( clReleaseMemObject( pCL.F ) ) ;
}
void write_data_to_device ( data_struct * data , cl_command_queue queue , cl_event * write_event ) {
    
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
    
    // grid
    CL_CHECK ( clEnqueueWriteBuffer( queue, gCL.X,  asynchronous, offset,  gCL.X_bytes, g.X, 0, NULL, write_event   ) ) ;
    
    // particle
    CL_CHECK ( clEnqueueWriteBuffer( queue, pCL.X,  asynchronous, offset,  pCL.X_bytes, p.X, 0, NULL, write_event+1 ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, pCL.V,  asynchronous, offset,  pCL.V_bytes, p.V, 0, NULL, write_event+2 ) ) ;
}
void read_data_to_host ( data_struct * data , cl_command_queue queue , cl_event_struct wait ) {
    
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
      
    // particle
    CL_CHECK ( clEnqueueReadBuffer( queue, pCL.X,  asynchronous, offset,  pCL.X_bytes, p.X, wait.count, wait.array , NULL ) ) ;
    CL_CHECK ( clEnqueueReadBuffer( queue, pCL.V,  asynchronous, offset,  pCL.V_bytes, p.V, wait.count, wait.array , NULL ) ) ;
    CL_CHECK ( clEnqueueReadBuffer( queue, pCL.F,  asynchronous, offset,  pCL.F_bytes, p.F, wait.count, wait.array , NULL ) ) ;
}
#undef p
#undef g
#undef pCL
#undef gCL