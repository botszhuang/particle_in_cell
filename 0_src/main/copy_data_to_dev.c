#include <main_struct.h>
#include <pthread.h>

#define OFFSET_0 0 

void copy_data_to_dev ( cl_command_queue io_queue , particle_struct * P , grid2_struct *grid , cl_event * dummy_event ){

    puts("### Writing grid and particle data to the device ......") ;

    CL_CHECK ( clEnqueueWriteBuffer( io_queue, P->dev_X[1].buffer , 
                                ASYNCHRONOUS, 0, 
                                P->dev_X[1].bytes , P->X[0] , 
                                1, dummy_event, &( P->dev_X[1].io ) ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( io_queue, P->dev_V[0].buffer , 
                                ASYNCHRONOUS, 0, 
                                P->dev_V[0].bytes , P->V[0] , 
                                1, dummy_event, &( P->dev_V[1].io ) ) ) ;


}

