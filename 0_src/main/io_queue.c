#include <main_struct.h>
#include <pthread.h>

pthread_mutex_t printLocker = PTHREAD_MUTEX_INITIALIZER; 

void myCall( dimension_2D_struct * p , const unsigned int pN, const char * fname ) {
   
    pthread_mutex_lock( &printLocker ); 
      
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

    myCall( x->p , x->number , x->fname ) ;
 
    CL_CHECK ( clSetUserEventStatus( x->print , CL_COMPLETE ) ) ;
    free ( x ) ;

}

void io_and_print( buffer_struct * Xcurrent ,
                   cl_event * LF_X , 
                   char * str0 ,
                   const unsigned int i , 
                   const unsigned int LOOP_N ,
                   const unsigned int P_number ,
                   cl_command_queue io_queue ,
                   cl_context context
                    ){
  
    if ( ( i +1 )%LOOP_N == 0 ) {  
            
    cl_int ret = 0 ;
    cl_event new_io_event ; 
    cl_event * waitList = malloc ( 2 * sizeof ( cl_event ) );
    
    unsigned int j = 0 ;
    
    waitList [ j++ ] = Xcurrent->print ;
    waitList [ j++ ] = LF_X [ LOOP_N -1 ] ;
            
    CL_CHECK( clEnqueueReadBuffer( io_queue, Xcurrent->buffer , ASYNCHRONOUS, 0, 
                                           Xcurrent->bytes, Xcurrent->host , 
                                          j , waitList , &(new_io_event) ) );
    CL_CHECK( clReleaseEvent( Xcurrent->io )); 
    
    Xcurrent->io = new_io_event ;          

    Xcurrent->print = clCreateUserEvent( context, &ret); CL_CHECK(ret);

    float t = i ;
    if( !(strcmp (str0 , "V")) ) { t += 0.5 ; }

    myCall_struct * x =  calloc ( 1, sizeof ( myCall_struct ) ) ;

    x->number = P_number ;
    x->print  = Xcurrent->print ;
    x->p = Xcurrent->host ;    
    x->waitList = waitList ;
    sprintf ( x->fname , "output/%s_%03.1lf.tex", str0 , t+1 ) ;
    CL_CHECK( clSetEventCallback( Xcurrent->io, CL_COMPLETE, on_io_complete, (void*)x ) );  
}        

}
