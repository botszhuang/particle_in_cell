#include <main_struct.h>
#include <pthread.h>

typedef struct {
    char str0[32];
    dimension_2D_struct * p ;
    unsigned int number ;
    unsigned int i ; 
    cl_event print ;
}myCall_struct;


#define ASYNCHRONOUS CL_FALSE 
pthread_mutex_t printLocker = PTHREAD_MUTEX_INITIALIZER; 
platform_struct gpu ;

void myCall( dimension_2D_struct * p , const unsigned int pN , const char * str0 , const unsigned int i ) ;
void CL_CALLBACK on_io_complete(cl_event event, cl_int status, void* user_data) ;


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

    grid_struct grid ;
    grid_cl_mem_struct gCL ;
    sync_cl_Struct sync[ 2 ] ;
    
    cl_command_queue io_queue  ;
    cl_command_queue dev_queue  ;

    const unsigned int LOOP_N = 6 ;
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
    printf ( "N = %i\n" , THIS.p.number ) ;

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

    puts("### Writing grid and particle data to the device ......") ;
    const unsigned int OFFSET_0 = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, gCL.X,       ASYNCHRONOUS, OFFSET_0,  gCL.X_bytes, grid.X, 1, &dummy_event, &io_GX ) ) ;   
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, THAT.pCL.X,  ASYNCHRONOUS, OFFSET_0,  THAT.pCL.bytesX , THAT.p.X, 1, &dummy_event, &( THAT.ioX ) ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, THAT.pCL.V,  ASYNCHRONOUS, OFFSET_0,  THAT.pCL.bytesV , THAT.p.V, 1, &dummy_event, &( THAT.ioV ) ) ) ;

    puts("### Running kernels ......") ;
    // leap frog step 0 : init V_half
    CL_CHECK ( clReleaseEvent ( dummy_event ) ) ;
    dummy_event = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ; 
    CL_CHECK ( clSetUserEventStatus(dummy_event, CL_COMPLETE) ) ; 
    cl_event LF_V_waitList [] = { THAT.ioX , THAT.ioV } ;                                
    run_leap_frog_init_kernel( &( THAT.leap_frog ) , ( myfloat * ) & dt , 
                               dev_queue , work_dim , global_work_offset_0 ,  global_work_size , local_work_size ,
                               2 , LF_V_waitList , &dummy_event ) ;

    cl_event X_waitlist [2] ;
                 
    for ( unsigned int i = 0 ; i < 12 ; i++ , current_T += dt , bufIndex = !bufIndex ) { 
        inv = ! bufIndex ;

        printf("LOOP:%i\n", i ) ;
        THIS.i = i ;
        // --------------------------------------------------------------------------
        // Queue: dev_queue
        // --------------------------------------------------------------------------

        // leap frog step 1 :
        puts("### Running LFX ......") ; fflush( stdout ) ;
        const unsigned int k = i % LOOP_N ;

        if ( k == 0 ) { X_waitlist [0] = dummy_event ; }
        else {          X_waitlist [0] = LF_V [ ( k -1 ) ] ; ;
        }

        X_waitlist [1] = THIS.ioX ;
 
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.leap_frog.X, work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , X_waitlist , & (LF_X [ k ] ) ) ) ;      

                                 

        // leap frog step 2 :   a = A(x)  compute A
        cl_event FORCE_waitlist [] = { LF_X [ k ] , THIS.ioF } ; 
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.force.G , work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , FORCE_waitlist , & ( GET_FORCE [ k ] ) ) ) ;

        // leap frog step 3 :
        cl_event V_waitlist[] = { GET_FORCE [ k ] , THIS.ioV  } ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.leap_frog.V_half, work_dim, global_work_offset_0, global_work_size, local_work_size,
                                         2 , V_waitlist  , & ( LF_V [ k ] ) ) ) ;                              

        // --------------------------------------------------------------------------
        // Queue: io_queue
        // --------------------------------------------------------------------------
        if ( ( i+1 )% LOOP_N == 0 ) { 

            fflush( stdout ) ;

            CL_CHECK( clWaitForEvents ( LOOP_N , LF_X      )) ;
            CL_CHECK( clWaitForEvents(1, &THIS.printX)); 
            CL_CHECK( clReleaseEvent(THIS.ioX)); 
            CL_CHECK( clEnqueueReadBuffer( io_queue, THIS.pCL.X, ASYNCHRONOUS, OFFSET_0, THIS.pCL.bytesX, THIS.p.X, 1, &(THIS.printX) , &(THIS.ioX) ) );
            THIS.printX = clCreateUserEvent(gpu.context, &ret); CL_CHECK(ret);

            CL_CHECK( clWaitForEvents ( LOOP_N , GET_FORCE )) ;
            CL_CHECK( clWaitForEvents(1, &THIS.printF));
            CL_CHECK( clReleaseEvent(THIS.ioF)); 
            CL_CHECK( clEnqueueReadBuffer( io_queue, THIS.pCL.F, ASYNCHRONOUS, OFFSET_0, THIS.pCL.bytesF, THIS.p.F, 1, &(THIS.printF) , &(THIS.ioF) ) );
            THIS.printF = clCreateUserEvent(gpu.context, &ret); CL_CHECK(ret);

            CL_CHECK( clWaitForEvents ( LOOP_N , LF_V      )) ;
            CL_CHECK( clWaitForEvents(1, &THIS.printV));       
            CL_CHECK( clReleaseEvent(THIS.ioV)); 
            CL_CHECK( clEnqueueReadBuffer( io_queue, THIS.pCL.V, ASYNCHRONOUS, OFFSET_0, THIS.pCL.bytesV, THIS.p.V, 1, &(THIS.printV) , &(THIS.ioV) ) );
            CL_CHECK( clReleaseEvent(THIS.printV) );
            THIS.printV = clCreateUserEvent(gpu.context, &ret); CL_CHECK(ret);
            
            for (unsigned int j = 0; j < LOOP_N; j++) {
                clReleaseEvent(LF_X     [j]);     
                clReleaseEvent(GET_FORCE[j]);  
                clReleaseEvent(LF_V     [j]);       
            }
    

            myCall_struct * x =  calloc ( 1, sizeof ( myCall_struct ) ) ;
            myCall_struct * v =  calloc ( 1, sizeof ( myCall_struct ) ) ;
            myCall_struct * f =  calloc ( 1, sizeof ( myCall_struct ) ) ;

            x->i = i ;
            v->i = i ;
            f->i = i ;

            x->number = THIS.p.number ;
            v->number = THIS.p.number ;
            f->number = THIS.p.number ;

            x->print  = THIS.printX ;
            v->print  = THIS.printV ;
            f->print  = THIS.printF ;

            x->p = THIS.p.X ;
            v->p = THIS.p.V ;
            f->p = THIS.p.F ;
            
            strcpy ( x->str0 , "X" ) ;
            strcpy ( v->str0 , "V" ) ;
            strcpy ( f->str0 , "F" ) ;
     
            CL_CHECK( clSetEventCallback( THIS.ioX, CL_COMPLETE, on_io_complete, (void*)x ) );
            CL_CHECK( clSetEventCallback( THIS.ioF, CL_COMPLETE, on_io_complete, (void*)f ) );
            CL_CHECK( clSetEventCallback( THIS.ioV, CL_COMPLETE, on_io_complete, (void*)v ) );

        }
        // --------------------------------------------------------------------------
            
    }

    fflush( stdout ) ;

    for (int b = 0; b < 2; b++) {
        CL_CHECK ( clWaitForEvents(1, &sync[b].ioX) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].ioV) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].ioF) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].printX) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].printV) );
        CL_CHECK ( clWaitForEvents(1, &sync[b].printF) );
    }


    #undef THIS
    #undef THAT


    // Now, force the main thread to acquire the lock. 
    // This will block until the callback finishes 'myCall' and unlocks.
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
    }
    free_grid_dev_memory ( & gCL ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

void myCall( dimension_2D_struct * p , const unsigned int pN, const char * str0 , const unsigned int i ) {
   
    printf( "%s: %i\n", str0 , i ) ;    fflush( stdout ) ;
    pthread_mutex_lock( &printLocker ); 
    
    char fname [128] ;
    sprintf ( fname , "output/%s_%i.tex\n", str0 , i ) ; 
   
    FILE * fptr = fopen ( fname , "w" ) ;
   
    fprintf ( fptr , "#x , y\n" ) ;
    
    for ( size_t i = 0 ; i < pN ; i++ ) { fprintf ( fptr , "%e %e\n", p[i].x , p[i].y ) ;  }

    fflush( fptr ) ;
    fclose ( fptr ) ;

    pthread_mutex_unlock( &printLocker ); 
     
}
void CL_CALLBACK on_io_complete(cl_event event, cl_int status, void* user_data) {

    if (status != CL_COMPLETE) {  
        fprintf(stderr, "Error: OpenCL event failed with status %d\n", status);
        return ;
    }

    myCall_struct* x = (myCall_struct*)user_data;

    myCall( x->p , x->number , x->str0 , x->i ) ;
 
    CL_CHECK ( clSetUserEventStatus( x->print , CL_COMPLETE ) ) ;
    free ( x ) ;

}
