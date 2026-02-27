#include <main_struct.h>

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
    printf("-----------------------\n"); 
    printf("Hello world!\n") ;

    platform_struct gpu ;
    input_tex_tag_struct input_tag ;
    
    myfloat current_T  = 0 ;
    const myfloat dt = 0.1 ;

    particle_struct particle ;
    grid_struct grid ;

    particle_cl_mem_struct pCL ;
    grid_cl_mem_struct gCL ;

    leap_frog_kernel_struct leap_frog ;
    force_kernel_struct force ;
    
    cl_command_queue io_queue  ;
    cl_command_queue dev_queue  ;
    
    cl_uint work_dim = 1 ;
    size_t global_work_offset_0[1] = {0} ;
    size_t global_work_size[1] = {1} ;
    size_t local_work_size[1] = {1} ;

    // --------------------------------------------------------------------------

    initialize_the_platform ( & gpu ) ;
    get_input_tex ( &input_tag , inputFile ) ;
    get_data_profile ( &grid , &particle , &input_tag ) ;

    // --------------------------------------------------------------------------
    cl_int ret = 0 ;

    // events
    cl_event io_GX ;
    cl_event io_X ;
    cl_event io_V ;
    cl_event io_F      = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ;   
    cl_event LF_V      = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ;
    cl_event LF_X      = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ;
    cl_event GET_FORCE = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ;
    
    ret = clSetUserEventStatus( io_F      , CL_COMPLETE ) ; CL_CHECK ( ret ) ;
    ret = clSetUserEventStatus( LF_X      , CL_COMPLETE ) ; CL_CHECK ( ret ) ;
    ret = clSetUserEventStatus( LF_V      , CL_COMPLETE ) ; CL_CHECK ( ret ) ;
    ret = clSetUserEventStatus( GET_FORCE , CL_COMPLETE ) ; CL_CHECK ( ret ) ;

    // --------------------------------------------------------------------------
    puts("### Allocating memory on the device ......") ;
    create_dev_memory ( &gCL , &pCL , grid , particle , gpu ) ;

    puts("### Creating kernels ......") ;
    create_leap_frog_kernel ( &leap_frog, gpu,  work_dim, global_work_offset_0, global_work_size, local_work_size ) ; 
    create_force_G_kernel ( &force, gpu,  work_dim, global_work_offset_0, global_work_size, local_work_size ) ; 

    // set kernels' args
    puts("### Setting the arguments for kernels ......") ;
    set_leap_frog_kernel_args ( & leap_frog , &( pCL.X ), &( pCL.V ), &( pCL.F ) , &( particle.number ), (double *) &( dt ) ) ;
    set_force_G_kernel_args ( & force , &( pCL.F ), &( particle.number ) ) ;

    // --------------------------------------------------------------------------
    create_queue_in_order     ( &io_queue  , gpu.context , gpu.devices[0] ) ;
    create_queue_out_of_order ( &dev_queue , gpu.context , gpu.devices[0] ) ;

    puts("### Writing grid and particle data to the device ......") ;
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset_0 = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, gCL.X,  asynchronous, offset_0,  gCL.X_bytes, grid.X,     0, NULL, &io_GX ) ) ;   
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, pCL.X,  asynchronous, offset_0,  pCL.X_bytes, particle.X, 0, NULL, &io_X ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, pCL.V,  asynchronous, offset_0,  pCL.V_bytes, particle.V, 0, NULL, &io_V ) ) ;

    // --------------------------------------------------------------------------
    puts("### Running kernels ......") ;
    
    // leap frog step 0 : init V_half
    cl_event LF_V_waitList [] = { io_X , io_V } ;
    CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, leap_frog.V_half, 
                                       work_dim, global_work_offset_0, global_work_size, local_work_size,
                                       2 , LF_V_waitList , &LF_V ) ) ;                                   
                                      
    for ( unsigned int i = 0 ; i < 10 ; i++ , current_T += dt ) { 
        
        printf("LOOP:%i\n", i ) ;

        // --------------------------------------------------------------------------
        // leap frog step 1 :
        puts("### Running LFX ......") ;
        cl_event LF_X_waitlist [] = { LF_V , io_X } ;
        CL_CHECK ( clReleaseEvent ( LF_X ) ) ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, leap_frog.X, work_dim, global_work_offset_0, global_work_size, local_work_size,
                                       2 , LF_X_waitlist , & LF_X ) ) ;       
        if ( ( i+1 )% 5 == 0 ) { 
            printf ( "T= %lf, [%i] READING X...\n", current_T ,  i ) ;
            CL_CHECK ( clReleaseEvent ( io_X ) ) ;
            CL_CHECK ( clEnqueueReadBuffer( io_queue, pCL.X, asynchronous, offset_0, pCL.X_bytes, particle.X, 1 , &LF_X , & io_X ) ) ;
        }
        // --------------------------------------------------------------------------                
        // leap frog step 2 :   a = A(x)  compute A
        cl_event FORCE_waitlist [] = { LF_X , io_F } ;
        CL_CHECK ( clReleaseEvent ( GET_FORCE ) ) ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, force.G , work_dim, global_work_offset_0, global_work_size, local_work_size,
                                       2 , FORCE_waitlist , & GET_FORCE ) ) ;
        if ( ( i+1 )% 5 == 0 ) { 
            printf ( "T= %lf, [%i] READING F...\n", current_T ,  i ) ;
            CL_CHECK ( clReleaseEvent ( io_F ) ) ;
            CL_CHECK ( clEnqueueReadBuffer( io_queue, pCL.F, asynchronous, offset_0, pCL.F_bytes, particle.F, 1 , &GET_FORCE , & io_F ) ) ;
        }  

        // --------------------------------------------------------------------------
        // leap frog step 3 :
        cl_event LF_V_waitlist [] = { GET_FORCE , io_V } ;
        CL_CHECK ( clReleaseEvent ( LF_V ) ) ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, leap_frog.V_half, work_dim, global_work_offset_0, global_work_size, local_work_size,
                                       2 , LF_V_waitlist , & LF_V ) ) ;                              
        if ( ( i+1 )% 5 == 0 ) { 
            printf ( "T= %lf, [%i] READING V...\n", current_T ,  i ) ;
            CL_CHECK ( clReleaseEvent ( io_V ) );
            CL_CHECK ( clEnqueueReadBuffer( io_queue, pCL.V, asynchronous, offset_0, pCL.V_bytes, particle.V, 1 , &LF_V , & io_V ) ) ;
        }
        // --------------------------------------------------------------------------                  
          
    }
    puts("### Waiting for the kernels to finish ......") ;
    cl_event READ_host [] = { io_X , io_V , io_F } ;
    clWaitForEvents( 3 , READ_host ) ; 
    // --------------------------------------------------------------------------
    
    free_leap_frog_kernel ( & leap_frog ) ;
    free_force_G_kernel ( & force ) ;
    free_dev_memory ( & gCL , & pCL ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

