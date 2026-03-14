#include <main_struct.h>

platform_struct gpu ;

const unsigned int LOOP_N = 100 ;
const unsigned int LOOP_END = 500 ;

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
    printf("-----------------------\n"); 
    printf("Hello world!\n") ;

    input_tex_tag_struct input_tag ;
    
    myfloat current_T  = 0 ;
    const myfloat dt = 0.1 ;
    printf ("dt = %lf\n" , dt ) ;

    grid_struct grid ;
    grid_cl_mem_struct gCL ;
    sync_cl_Struct sync[ 2 ] ;
    
    cl_command_queue io_queue  ;
    cl_command_queue dev_queue  ;

    cl_event LF_V      [ LOOP_N ] ;
    cl_event LF_X      [ LOOP_N ] ;
    cl_event GET_FORCE [ LOOP_N ] ;
    
    cl_uint work_dim = 1 ;
    size_t global_work_offset_0[1] = {0} ;
    size_t global_work_size[1] = {2} ;
    size_t local_work_size[1] = {2} ;

    // --------------------------------------------------------------------------
    bool bufIndex = 0 ;
    bool inv = 1 ;
    #define THIS (sync[bufIndex])
    #define THAT (sync[inv])

    initialize_the_platform ( & gpu ) ;
    
    get_input_tex ( &input_tag , inputFile ) ;
    get_grid_profile ( &grid , &input_tag ) ;
    get_particle_profile ( &(THAT.p) , &input_tag ) ;
    THIS.p.number = THAT.p.number ;
    THIS.p.X = malloc ( THAT.p.number * sizeof ( THAT.p.X[0] ) ) ;
    THIS.p.F = malloc ( THAT.p.number * sizeof ( THAT.p.F[0] ) ) ;
    THIS.p.V = malloc ( THAT.p.number * sizeof ( THAT.p.V[0] ) ) ;

    create_grid_dev_memory ( & gCL ,  grid , gpu ) ;
    create_particle_dev_memory( &(THIS) , &gpu ) ;
    create_particle_dev_memory( &(THAT) , &gpu ) ;

    init_double_buffer_kernels_and_args( sync ,  &gpu , (myfloat*)  & dt );

    // events
    cl_int ret = 0 ;

    cl_event io_GX = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ;
    init_double_buffer_events ( sync , & gpu ) ;

    cl_event dummy_event = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ; 
                     ret = clSetUserEventStatus(dummy_event, CL_COMPLETE) ; CL_CHECK( ret ) ; 

    //queue
    create_queue_in_order     ( &io_queue  , gpu.context , gpu.devices[0] ) ;
    create_queue_out_of_order ( &dev_queue , gpu.context , gpu.devices[0] ) ;
 
    // --------------------------------------------------------------------------
    puts("### Running kernels ......") ;
    // leap frog step 0 : init V_half
    puts("### Writing grid and particle data to the device ......") ;
    const unsigned int OFFSET_0 = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, gCL.X,       ASYNCHRONOUS, OFFSET_0,  gCL.X_bytes, grid.X, 1, &dummy_event, &io_GX ) ) ;   
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, THAT.pCL.X,  ASYNCHRONOUS, OFFSET_0,  THAT.pCL.bytesX , THAT.p.X, 1, &dummy_event, &( THAT.X.io ) ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, THIS.pCL.V,  ASYNCHRONOUS, OFFSET_0,  THAT.pCL.bytesV , THAT.p.V, 1, &dummy_event, &( THAT.V.io ) ) ) ;


    
    cl_event init_F_waitList [] = { THAT.X.io , THAT.V.io } ;  
    cl_event init_F ;     
    //Compute acceleration at the initial position
    CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THAT.force.G , work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , init_F_waitList , & ( init_F) ) ) ; 
    //Compute velocity at the initial position                      
    run_leap_frog_init_kernel( &( THAT.leap_frog ) , ( myfloat * ) & dt , 
                               dev_queue , work_dim , global_work_offset_0 ,  global_work_size , local_work_size ,
                               1 , &(init_F) , &dummy_event ) ;

    cl_event X_waitlist     [2] ;
    cl_event FORCE_waitlist [2] ; 
    cl_event V_waitlist     [2] ; 
    unsigned int k = 0 ;

    for ( unsigned int i = 0 ; i < LOOP_END ; i++ , current_T += dt , bufIndex = !bufIndex ) { 
        inv = ! bufIndex ;

        if ( !(i%LOOP_N) ) { printf("LOOP:%i\n", i ) ; }
        THIS.i = i ;
        // --------------------------------------------------------------------------
        // Queue: dev_queue
        // --------------------------------------------------------------------------

        // leap frog step 1 :
        
        k = i % LOOP_N ;

        if ( k == 0 ) { X_waitlist [0] = dummy_event ; }
        else {          X_waitlist [0] = LF_V [ ( k -1 ) ] ; ;
        }
        X_waitlist [1] = THIS.X.io ;
 
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.leap_frog.X, work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , X_waitlist , & (LF_X [ k ] ) ) ) ;      
        if ( ( i+1 )% LOOP_N == 0 ) { 
            run_io_queue ( &io_queue  , LF_X , & gpu , &( THIS.X ) , THIS.pCL.X , THIS.pCL.bytesX , THIS.p.X , THIS.p.number ,  LOOP_N , i , 0 , "X" ); 
        }                                 
        // leap frog step 2 :   a = A(x)  compute A
        FORCE_waitlist[0] = LF_X [ k ] ;
        FORCE_waitlist[1] = THIS.F.io ; 
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.force.G , work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , FORCE_waitlist , & ( GET_FORCE [ k ] ) ) ) ;
        if ( ( i+1 )% LOOP_N == 0 ) { 
            run_io_queue ( &io_queue  , GET_FORCE , & gpu , &( THIS.F ) , THIS.pCL.F , THIS.pCL.bytesF , THIS.p.F , THIS.p.number ,  LOOP_N , i , 0 , "F" ); 
        }     
        // leap frog step 3 :
        V_waitlist[0] = GET_FORCE [ k ] ;
        V_waitlist[1] = THIS.V.io ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.leap_frog.V_half, work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , V_waitlist  , & ( LF_V [ k ] ) ) ) ;   
 
        if ( !(( i+1 )% LOOP_N) ) {          
            run_io_queue ( &io_queue  , LF_V , & gpu , &( THIS.V ) , THIS.pCL.V , THIS.pCL.bytesV , THIS.p.V , THIS.p.number , LOOP_N , i , 0.5 , "V" );

            freeEventArray( LOOP_N , LF_X      ) ;
            freeEventArray( LOOP_N , GET_FORCE ) ;
            freeEventArray( LOOP_N , LF_V      ) ;
        
        }
        // --------------------------------------------------------------------------
           
    }

    fflush( stdout ) ;

    for (int b = 0; b < 2; b++) {
        CL_CHECK ( clWaitForEvents(1, &sync[b].X.io) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].V.io) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].F.io) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].X.print) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].V.print) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].F.print) );
    }


    #undef THIS
    #undef THAT


    pthread_mutex_lock(&printLocker); 
    pthread_mutex_unlock(&printLocker);
    pthread_mutex_destroy( &printLocker ) ;

    CL_CHECK ( clFlush  ( dev_queue ) );
    CL_CHECK ( clFinish ( dev_queue ) );
    CL_CHECK ( clFlush  ( io_queue ) );
    CL_CHECK ( clFinish ( io_queue ) );

    puts("### end of the kernels ......") ;

    // --------------------------------------------------------------------------
    for ( unsigned int i = 0 ; i < 2 ; i++ ){    
        free_leap_frog_kernel    ( & ( sync[i].leap_frog ) ) ;
        free_force_G_kernel      ( & ( sync[i].force     ) ) ;
        free_particle_dev_memory ( & ( sync[i].pCL       ) ) ;
        clReleaseEvent ( sync[i].X.io ) ;
        clReleaseEvent ( sync[i].F.io ) ;
        clReleaseEvent ( sync[i].V.io ) ;
        clReleaseEvent ( sync[i].X.print ) ;
        clReleaseEvent ( sync[i].F.print ) ;
        clReleaseEvent ( sync[i].V.print ) ;

    }

    free_grid_dev_memory ( & gCL ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

