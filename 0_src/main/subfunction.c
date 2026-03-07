#include <main_struct.h>

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ){
    printf("### Reading %s ......\n" , inputFile ) ;
    init_read_input_tex( inputFile ) ;
    input_tag->grid_file = read_input_tex( "grid_file" ) ;
    input_tag->particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag->particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;
}
void get_grid_profile ( grid_struct * g , input_tex_tag_struct * tag ) {

    puts("### Reading grid profiles ......") ;

    // grid
    g->number = read_2D( & ( g->X ) , tag->grid_file ) ;    

}
void get_particle_profile ( particle_struct * p , input_tex_tag_struct * tag ) {

    puts("### Reading particle profiles ......") ;

    int N = 0 ;

    // particle
    N = read_2D( & ( p->X ) , tag->particle_position_file ) ;
    N = read_2D( & ( p->V ) , tag->particle_velocity_file ) ;

    p->F = calloc (  N , sizeof ( p->F[0] ) ) ;
    
    p->number = ( unsigned int ) N ;

    printf ( "N = %i\n" , N ) ;


    print_2D_list( p->X , p->number ) ;
    //print_2D_list( particles->velocity , particles->number ) ;

    if ( p->X == NULL || p->V == NULL || p->F == NULL) {
        fprintf(stderr, "Host memory allocation failed!\n");
        exit (1) ;
    }
}

void create_grid_dev_memory ( grid_cl_mem_struct * gCL , const grid_struct g , platform_struct gpu ) {

    cl_int ret = 0 ;  
    
    // grid
    gCL->X_bytes = g.number * sizeof( grid_dimension ) ;
    gCL->X = clCreateBuffer( gpu.context , CL_MEM_READ_WRITE , gCL->X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

}
void free_grid_dev_memory ( grid_cl_mem_struct * gCL ) {
    
    // grid
    CL_CHECK( clReleaseMemObject( gCL->X ) ) ;
   
}

void create_particle_dev_memory( sync_cl_Struct * sync , platform_struct * gpu ){
    cl_int ret = 0 ;
   
    sync->pCL.bytesX = sync->p.number * sizeof ( sync->p.X[0] ) ;
    sync->pCL.bytesV = sync->p.number * sizeof ( sync->p.V[0] ) ;
    sync->pCL.bytesF = sync->p.number * sizeof ( sync->p.F[0] ) ;


    sync->pCL.X = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , sync->pCL.bytesX , NULL , &ret ) ; CL_CHECK( ret ) ;
    sync->pCL.V = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , sync->pCL.bytesV , NULL , &ret ) ; CL_CHECK( ret ) ;
    sync->pCL.F = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , sync->pCL.bytesF , NULL , &ret ) ; CL_CHECK( ret ) ;
    
}
void free_particle_dev_memory ( particle_cl_mem_struct * pCL ) {
    
    // particle
    CL_CHECK( clReleaseMemObject( pCL->X ) ) ;
    CL_CHECK( clReleaseMemObject( pCL->V ) ) ;
    CL_CHECK( clReleaseMemObject( pCL->F ) ) ;
}

void init_double_buffer_events ( sync_cl_Struct * sync , platform_struct * gpu ){
    
    cl_int ret = 0 ;

    for ( unsigned i = 0 ; i < 2  ; i++ ) {         

        sync[i].ioX    = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ;
        sync[i].ioV    = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ;
        sync[i].ioF    = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ;
        sync[i].printX = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ;
        sync[i].printV = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ;
        sync[i].printF = clCreateUserEvent( gpu->context , &ret ) ; CL_CHECK( ret ) ;

        CL_CHECK ( clSetUserEventStatus(sync[i].ioX, CL_COMPLETE) );
        CL_CHECK ( clSetUserEventStatus(sync[i].ioV, CL_COMPLETE) );
        CL_CHECK ( clSetUserEventStatus(sync[i].ioF, CL_COMPLETE) );

        CL_CHECK ( clSetUserEventStatus(sync[i].printX, CL_COMPLETE) );
        CL_CHECK ( clSetUserEventStatus(sync[i].printV, CL_COMPLETE) );
        CL_CHECK ( clSetUserEventStatus(sync[i].printF, CL_COMPLETE) );
    }

}
void init_double_buffer_kernels_and_args( sync_cl_Struct *sync ,  platform_struct * gpu , myfloat * dt ){
    
    unsigned int inv = 0 ;

    for ( unsigned i = 0 ; i < 2  ; i++ ) {   
        
        inv = (i +1) % 2 ;

        create_leap_frog_kernel   ( &( sync[i].leap_frog ) , gpu ) ; 
        create_force_G_kernel   ( & ( sync[i].force ) , gpu ) ; 

        set_leap_frog_kernel_args ( &( sync[i].leap_frog ) , 
                                &( sync[inv].pCL.X ), &( sync[inv].pCL.V ), &( sync[i].pCL.F ) , 
                                &( sync[i].p.number ) , (double *) dt , 
                                &( sync[i].pCL.X ) , &( sync[i].pCL.V ) ) ;

        set_force_G_kernel_args ( & ( sync[i].force ) , 
                                  & ( sync[inv].pCL.F ), &( sync[i].pCL.X ), 
                                  & ( sync[i].p.number ) , 
                                  & ( sync[i].pCL.F ) ) ;

 
    } 
}