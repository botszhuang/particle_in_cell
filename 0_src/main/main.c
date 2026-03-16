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

    cl_command_queue io_queue  ;
    cl_command_queue dev_queue  ;

    cl_event LF_V      [ LOOP_N ] ;
    cl_event LF_X      [ LOOP_N ] ;
    cl_event GET_FORCE [ LOOP_N ] ;
    
    size_t global_work_offset_0[1] = {0} ;
    size_t global_work_size[1] = {2} ;
    size_t local_work_size[1] = {1} ;

    work_dim_struct w ;
    w.dim = 1 ;
    w.global_offset = global_work_offset_0 ;
    w.global_size   = global_work_size ;
    w.local_size    = local_work_size ;

    // --------------------------------------------------------------------------
    initialize_the_platform ( & gpu ) ;
    
    get_input_tex ( &input_tag , inputFile ) ;

    particle_struct P ;
    grid2_struct Grid ;
    cl_event dummy_event ;
    leap_frog_kernel_struct leagfrog_kernels [ 2 ] ;
    cl_kernel force_kernels [ 2 ] ;
    
    get_grid_profile     ( & Grid , & input_tag , & gpu ) ;
    get_particle_profile ( & P    , & input_tag , & gpu ) ;
    create_and_set_event_CL_COMPLETE ( &dummy_event , & gpu ) ;

    init_leapfrog_kernels_and_args ( leagfrog_kernels , & P ,  & Grid , & gpu , ( myfloat * ) & dt  ) ;
    init_force_kernels_and_args( force_kernels , & P , & Grid , & gpu ) ;
    
    //queue
    create_queue_in_order     ( &io_queue  , gpu.context , gpu.devices[0] ) ;
    create_queue_out_of_order ( &dev_queue , gpu.context , gpu.devices[0] ) ;
 
    // --------------------------------------------------------------------------
    puts("### Running kernels ......") ;

    copy_data_to_dev ( io_queue , & P , & Grid , & dummy_event ) ;

    init_V_half ( dev_queue , &P , & Grid , force_kernels , leagfrog_kernels , & w , & dummy_event ,  ( myfloat * ) & dt  ) ;

    cl_event X_waitlist  [2] ;
    cl_event F_waitlist  [2] ;
    cl_event V_waitlist  [2] ;

    bool current = 0 ;
    bool previous = 0 ;

    unsigned int j = 0 ;
    unsigned int k = 0 ;
    for ( unsigned int i = 0 ; i < LOOP_END ; ++i , current = !current ) {

        if ( ((i+1)%LOOP_N) == 0 ) { printf ( "LOOP: %i\n" , i ) ; }

        previous = ! current ;

        k = i % LOOP_N ;
        j = 0 ;
        if ( k == 0 ) { X_waitlist [ j++ ] = dummy_event ; }
        else {          X_waitlist [ j++ ] = LF_V [ ( k -1 ) ] ; ;
        }
        X_waitlist[ j++ ] = P.dev_X[ previous ].io ;

        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, leagfrog_kernels[ current ].X , w.dim, w.global_offset , w.global_size, w.local_size, j , X_waitlist , &(LF_X[k]) ) ) ;  
        io_and_print( & ( P.dev_X [ current ] ) , LF_X , "X" , i , LOOP_N , P.number , io_queue , gpu.context ) ;
                                           
        j = 0 ;
        F_waitlist[ j++ ] = LF_X [ k ] ;
        F_waitlist[ j++ ] = P.dev_F[ current ].io ;                                     
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, force_kernels[ current ], w.dim, w.global_offset , w.global_size, w.local_size, j , F_waitlist , &(GET_FORCE[k]) ) ) ;      
        io_and_print( & ( P.dev_F [ current ] ) , GET_FORCE , "F" , i , LOOP_N , P.number , io_queue , gpu.context ) ;
                                            
        j = 0 ;
        V_waitlist[ j++ ] = GET_FORCE [ k ] ;
        V_waitlist[ j++ ] = P.dev_V[ current ].io ;                                     
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, leagfrog_kernels[ current ].V_half , w.dim, w.global_offset , w.global_size, w.local_size, j , V_waitlist , &(LF_V[k]) ) ) ; 
        io_and_print( & ( P.dev_V [ current ] ) , LF_V , "V" , i , LOOP_N , P.number , io_queue , gpu.context ) ;
                                         
        if ( ((i+1)%LOOP_N) == 0 ) {
            
            clEnqueueBarrierWithWaitList ( dev_queue , LOOP_N , LF_X      , NULL ) ;
            freeEventArray( LOOP_N , LF_X      ) ;

            clEnqueueBarrierWithWaitList ( dev_queue , LOOP_N , GET_FORCE , NULL ) ;
            freeEventArray( LOOP_N , GET_FORCE ) ;

            clEnqueueBarrierWithWaitList ( dev_queue , LOOP_N , LF_V      , NULL ) ;
            freeEventArray( LOOP_N , LF_V      ) ;
        }                                 
    }
    
    pthread_mutex_lock(&printLocker); 
    pthread_mutex_unlock(&printLocker);
    pthread_mutex_destroy( &printLocker ) ;

    CL_CHECK ( clFlush  ( dev_queue ) );
    CL_CHECK ( clFinish ( dev_queue ) );
    CL_CHECK ( clFlush  ( io_queue ) );
    CL_CHECK ( clFinish ( io_queue ) );

    return EXIT_SUCCESS;
}

