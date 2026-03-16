#include <main_struct.h>

void init_leapfrog_kernels_and_args( leap_frog_kernel_struct * kernels ,
                                     particle_struct * p , 
                                     grid2_struct * grid , 
                                     platform_struct * gpu , 
                                     myfloat * dt ){
    
    unsigned int inv = 0 ;

    leap_frog_arg_struct args ;
    args.dt = dt ;

    for ( unsigned i = 0 ; i < 2  ; i++ ) {   
        
        inv = (i +1) % 2 ;
        args.position = &(p->dev_X[inv].buffer) ;
        args.velocity = &(p->dev_V[inv].buffer) ;
        args.F        = &(p->dev_F[ i ].buffer) ;
        args.particle_N = &(p->number) ;
        args.positionOUT = &(p->dev_X[ i ].buffer) ;
        args.velocityOUT = &(p->dev_V[ i ].buffer) ;

        create_leap_frog_kernel   ( kernels+i ,   gpu  ) ; 
        set_leap_frog_kernel_args ( kernels+i , & args ) ;
 
    } 
}
