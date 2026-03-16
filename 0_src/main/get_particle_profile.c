#include <main_struct.h>

void get_particle_profile ( particle_struct * p , input_tex_tag_struct * tag , platform_struct * gpu ) {

    puts("### Reading particle profiles ......") ;
  
    cl_int ret = 0 ;

    int N = 0 ;
    particle_dimension * Xptr ;
    particle_dimension * Vptr ;

    N = read_2D( & Xptr , tag->particle_position_file ) ;
    if ( Xptr == NULL ) { fprintf(stderr, "Host memory allocation failed!\n"); exit (1) ;  }

    N = read_2D( & Vptr , tag->particle_velocity_file ) ;
    if ( Vptr == NULL ) { fprintf(stderr, "Host memory allocation failed!\n"); exit (1) ;  }

    p->number = N ;
    size_t bytes = N * sizeof ( Vptr[0] ) ;
    
    p->X[0] = Xptr ;
    p->X[1] = calloc ( N , sizeof ( Xptr[0] ) ) ;   

    p->V[0] = Vptr ;
    p->V[1] = calloc ( N , sizeof ( Vptr[0] ) ) ;

    for ( unsigned int i = 0 ; i < 2 ; ++i ) {
        p->F[i] = calloc ( N , sizeof ( Vptr[0] ) ) ;

        p->dev_X[i].bytes = bytes ; 
        p->dev_V[i].bytes = bytes ; 
        p->dev_F[i].bytes = bytes ; 
        
        p->dev_X[i].host = p->X[i] ;
        p->dev_V[i].host = p->V[i] ;
        p->dev_F[i].host = p->F[i] ;

        p->dev_X[i].buffer = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
        p->dev_V[i].buffer = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
        p->dev_F[i].buffer = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

        create_and_set_event_CL_COMPLETE ( &( p->dev_F[i].io    ), gpu ) ;
        create_and_set_event_CL_COMPLETE ( &( p->dev_X[i].print ), gpu ) ; 
        create_and_set_event_CL_COMPLETE ( &( p->dev_V[i].print ), gpu ) ; 
        create_and_set_event_CL_COMPLETE ( &( p->dev_F[i].print ), gpu ) ; 
    }
    
    create_and_set_event_CL_COMPLETE ( &( p->dev_X[0].io    ), gpu ) ;
    create_and_set_event_CL_COMPLETE ( &( p->dev_V[0].io    ), gpu ) ;

    printf ( "N = %i\n" , N ) ;
    puts("X:");
    print_2D_list( p->X[0] , N ) ;
    puts("V:");
    print_2D_list( p->V[0] , N ) ;

}
