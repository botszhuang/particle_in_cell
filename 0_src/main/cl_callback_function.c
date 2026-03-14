#include <main_struct.h>
#include <pthread.h>

pthread_mutex_t printLocker = PTHREAD_MUTEX_INITIALIZER; 

void myCall( dimension_2D_struct * p , const unsigned int pN, const char * str0 , const float i ) {
   
    pthread_mutex_lock( &printLocker ); 
    
    char fname [128] ;
    sprintf ( fname , "output/%s_%03.1lf.tex", str0 , i+1 ) ; 
   
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

    free ( x->waitList ) ;

    myCall( x->p , x->number , x->str0 , x->i ) ;
 
    CL_CHECK ( clSetUserEventStatus( x->print , CL_COMPLETE ) ) ;
    free ( x ) ;

}

void run_io_queue ( cl_command_queue * io_queue  , cl_event * funcEvent , platform_struct * gpu , io_event_struct * X , 
                    cl_mem pCLX , const size_t pCL_bytesX , 
                    particle_dimension * pX , const size_t p_number , 
                    const unsigned int LOOP_N , const unsigned int i , const float iplus , char * str0 ) {
    
    cl_int ret = 0 ;
    cl_event new_io_event ; 
    cl_event * waitList = malloc ( 2 * sizeof ( cl_event ) ) ;
    waitList [0] = X->print ;
    waitList [1] = funcEvent [ LOOP_N -1 ] ;
    
    CL_CHECK( clEnqueueReadBuffer( *io_queue, pCLX, ASYNCHRONOUS, 0, pCL_bytesX, pX, 2 , waitList , &(new_io_event) ) );
    CL_CHECK( clReleaseEvent((X->io))); 
    X->io = new_io_event ;
    X->print = clCreateUserEvent(gpu->context, &ret); CL_CHECK(ret);

    myCall_struct * x =  calloc ( 1, sizeof ( myCall_struct ) ) ;

    x->i = i + iplus ;
    x->number = p_number ;
    x->print  = X->print ;
    x->p = pX ;    
    x->waitList = waitList ;
    strcpy ( x->str0 , str0 ) ;
    CL_CHECK( clSetEventCallback( X->io, CL_COMPLETE, on_io_complete, (void*)x ) );
      
}

