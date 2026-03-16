#include <main_struct.h>

void init_V_half ( cl_command_queue dev_queue , particle_struct * p , grid2_struct * grid , 
                   cl_kernel * force_kernels , leap_frog_kernel_struct * leap_frog_kernel ,
                   work_dim_struct * w , cl_event * dummy_event , double * dt  ){
   
    // 0.2 : Compute Force at the initial position
    cl_event init_F_waitList [] = { p->dev_X[1].io , p->dev_V[1].io } ;  
    cl_event init_F ; 
    CL_CHECK ( clEnqueueNDRangeKernel( dev_queue,  force_kernels [1] , 
                                        w->dim, w->global_offset, w->global_size, w->local_size,
                                        2 , init_F_waitList , & ( init_F ) ) ) ; 

    // 0.3 : Compute velocity at the initial position                      
    run_leap_frog_init_kernel( leap_frog_kernel+1 , ( myfloat * ) dt , 
                               dev_queue , w->dim , w->global_offset ,  w->global_size , w->local_size ,
                               1 , & ( init_F ) , dummy_event ) ;
}

