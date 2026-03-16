#include <main_struct.h>

void init_force_kernels_and_args( cl_kernel * kernel , particle_struct * p , grid2_struct *grid , platform_struct * gpu ){
    
    unsigned int inv = 0 ;

    force_kernel_args_struct args ; 

    for ( unsigned i = 0 ; i < 2  ; i++ ) {   
        
        inv = (i +1) % 2 ;

        create_force_kernel   ( kernel +i , gpu ) ; 

        args.grid_N = &(grid->grid_total_number) ; 
        args.gird   = &(grid->dev_X.buffer ) ;
        args.particle_N = &( p->number ) ; 
        args.particle_X = &( p->dev_X[i].buffer   ) ;
        args.force      = &( p->dev_F[inv].buffer ) ;
        args.forceOUT   = &( p->dev_F[i].buffer   ) ;

        set_force_kernel_args ( kernel[i] , &args ) ;

 
    } 
}
