#include <main_struct.h>

#define ASYNCHRONOUS CL_FALSE 

platform_struct gpu ;

void myCall( dimension_2D_struct * p , const unsigned int pN , const char * str0 ) ;

void CL_CALLBACK on_readX_complete(cl_event event, cl_int status, void* user_data) ;
void CL_CALLBACK on_readF_complete(cl_event event, cl_int status, void* user_data) ;
void CL_CALLBACK on_readV_complete(cl_event event, cl_int status, void* user_data) ;

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
    cl_event dummy_event = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ; 
                     ret = clSetUserEventStatus(dummy_event, CL_COMPLETE) ; CL_CHECK( ret ) ; 
    cl_event io_GX = clCreateUserEvent( gpu.context , &ret ) ; CL_CHECK( ret ) ;
    init_double_buffer_events ( sync , & gpu ) ;

    //queue
    create_queue_in_order     ( &io_queue  , gpu.context , gpu.devices[0] ) ;
    create_queue_out_of_order ( &dev_queue , gpu.context , gpu.devices[0] ) ;
 
    // --------------------------------------------------------------------------

    puts("### Writing grid and particle data to the device ......") ;
    const unsigned int OFFSET_0 = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, gCL.X,       ASYNCHRONOUS, OFFSET_0,  gCL.X_bytes, grid.X, 1, &dummy_event, &io_GX ) ) ;   
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, THAT.pCL.X,  ASYNCHRONOUS, OFFSET_0,  THAT.pCL.bytesX , THAT.p.X, 1, &dummy_event, &( THAT.ioX ) ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, THAT.pCL.V,  ASYNCHRONOUS, OFFSET_0,  THAT.pCL.bytesV , THAT.p.V, 1, &dummy_event, &( THAT.ioV ) ) ) ;

    // --------------------------------------------------------------------------
    puts("### Running kernels ......") ;
    // leap frog step 0 : init V_half
    cl_event LF_V_waitList [] = { THAT.ioX , THAT.ioV } ;                                
    run_leap_frog_init_kernel( &( THAT.leap_frog ) , ( myfloat * ) & dt , 
                               dev_queue , work_dim , global_work_offset_0 ,  global_work_size , local_work_size ,
                               2 , LF_V_waitList , &THAT.LF_V ) ;
                     
    for ( unsigned int i = 0 ; i < 10 ; i++ , current_T += dt , bufIndex = !bufIndex ) { 
        inv = ! bufIndex ;

        printf("LOOP:%i\n", i ) ;
        THIS.i = i ;
        // --------------------------------------------------------------------------
        // Queue: dev_queue
        // --------------------------------------------------------------------------
        // leap frog step 1 :
        puts("### Running LFX ......") ;
        cl_event X_waitlist [] = { THAT.LF_V , THIS.ioX } ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.leap_frog.X, work_dim, global_work_offset_0, global_work_size, local_work_size, 2 , X_waitlist , & THIS.LF_X ) ) ;      
        CL_CHECK ( clReleaseEvent ( THAT.LF_V ) ) ;
        // leap frog step 2 :   a = A(x)  compute A
        cl_event FORCE_waitlist [] = { THIS.LF_X , THIS.ioF } ; 
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.force.G , work_dim, global_work_offset_0, global_work_size, local_work_size, 2 , FORCE_waitlist , & THIS.GET_FORCE ) ) ;
        CL_CHECK ( clReleaseEvent ( THIS.LF_X ) ) ; 
        // leap frog step 3 :
        cl_event V_waitlist[] = { THIS.GET_FORCE , THIS.ioV  } ;
        CL_CHECK ( clEnqueueNDRangeKernel( dev_queue, THIS.leap_frog.V_half, work_dim, global_work_offset_0, global_work_size, local_work_size, 2 , V_waitlist  , & THIS.LF_V ) ) ;                              
        CL_CHECK ( clReleaseEvent ( THIS.GET_FORCE ) ) ;

        // --------------------------------------------------------------------------
        // Queue: io_queue
        // --------------------------------------------------------------------------
        if ( ( i+1 )% 5 == 0 ) { 

            // 3. Define waitlists for the reads
            cl_event wX[] = { THIS.LF_X,      THIS.ioX , THIS.printX }; 
            cl_event wF[] = { THIS.GET_FORCE, THIS.ioF , THIS.printF }; 
            cl_event wV[] = { THIS.LF_V,      THIS.ioV , THIS.printV }; 

            cl_event new_ioX, new_ioF, new_ioV;

            // 4. Enqueue the Asynchronous Reads
            CL_CHECK( clEnqueueReadBuffer( io_queue, THIS.pCL.X, ASYNCHRONOUS, OFFSET_0, THIS.pCL.bytesX, THIS.p.X, 3, wX, &new_ioX ) );
            CL_CHECK( clEnqueueReadBuffer( io_queue, THIS.pCL.F, ASYNCHRONOUS, OFFSET_0, THIS.pCL.bytesF, THIS.p.F, 3, wF, &new_ioF ) );
            CL_CHECK( clEnqueueReadBuffer( io_queue, THIS.pCL.V, ASYNCHRONOUS, OFFSET_0, THIS.pCL.bytesV, THIS.p.V, 3, wV, &new_ioV ) );

            // 5. Set up Callbacks to trigger the prints
            CL_CHECK( clSetEventCallback( new_ioX, CL_COMPLETE, on_readX_complete, (void*)&THIS ) );
            CL_CHECK( clSetEventCallback( new_ioF, CL_COMPLETE, on_readF_complete, (void*)&THIS ) );
            CL_CHECK( clSetEventCallback( new_ioV, CL_COMPLETE, on_readV_complete, (void*)&THIS ) );

            // 6. Update the sync structure events
            clReleaseEvent(THIS.ioX); THIS.ioX = new_ioX;
            clReleaseEvent(THIS.ioF); THIS.ioF = new_ioF;
            clReleaseEvent(THIS.ioV); THIS.ioV = new_ioV;
            
        }
        // --------------------------------------------------------------------------
            
    }

    fflush( stdout ) ;


    for (int b = 0; b < 2; b++) {
        clWaitForEvents(1, &sync[b].printX);
        clWaitForEvents(1, &sync[b].printV);
        clWaitForEvents(1, &sync[b].printF);
        clWaitForEvents(1, &sync[b].ioX);
        clWaitForEvents(1, &sync[b].ioV);
        clWaitForEvents(1, &sync[b].ioF);
    }


    #undef THIS
    #undef THAT
    

    clFlush  ( dev_queue ) ;
    clFinish ( dev_queue ) ;
    clFlush  ( io_queue ) ;
    clFinish ( io_queue ) ;


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

void myCall( dimension_2D_struct * p , const unsigned int pN, const char * str0 ) {
   
    puts(str0);
    print_2D_list ( p , 2 ) ;

    fflush( stdout ) ;
     
}

void CL_CALLBACK on_readX_complete(cl_event event, cl_int status, void* user_data) {

    if (status != CL_COMPLETE) {  
        fprintf(stderr, "Error: OpenCL event failed with status %d\n", status);
        return ;
    }

    sync_cl_Struct* THIS = (sync_cl_Struct*)user_data;

    cl_int ret = 0 ;
    CL_CHECK( clReleaseEvent( THIS->printX ));
    THIS->printX = clCreateUserEvent(gpu.context, &ret); CL_CHECK(ret);
  
    myCall( THIS->p.X , THIS->p.number , __FUNCTION__ ) ;
  
    CL_CHECK ( clSetUserEventStatus( THIS->printX , CL_COMPLETE ) ) ;

}
void CL_CALLBACK on_readF_complete(cl_event event, cl_int status, void* user_data) {
    
    if (status != CL_COMPLETE) {  
        fprintf(stderr, "Error: OpenCL event failed with status %d\n", status);
        return ;
    }

    sync_cl_Struct* THIS = (sync_cl_Struct*)user_data;

    cl_int ret = 0 ;
    CL_CHECK( clReleaseEvent( THIS->printF ));
    THIS->printF = clCreateUserEvent(gpu.context, &ret); CL_CHECK(ret);
    
    myCall( THIS->p.F , THIS->p.number , __FUNCTION__ ) ;

    CL_CHECK ( clSetUserEventStatus( THIS->printF , CL_COMPLETE ) ) ;
}
void CL_CALLBACK on_readV_complete(cl_event event, cl_int status, void* user_data) {
    
    if (status != CL_COMPLETE) {  
        fprintf(stderr, "Error: OpenCL event failed with status %d\n", status);
        return ;
    }

    sync_cl_Struct* THIS = (sync_cl_Struct*)user_data;

    cl_int ret = 0 ;
    CL_CHECK( clReleaseEvent( THIS->printV ));
    THIS->printV = clCreateUserEvent(gpu.context, &ret); CL_CHECK(ret);

    myCall( THIS->p.V , THIS->p.number , __FUNCTION__ ) ;

    CL_CHECK ( clSetUserEventStatus( THIS->printV , CL_COMPLETE ) ) ;
}