#ifndef TEST_PARTICLE_MEM_C
#define TEST_PARTICLE_MEM_C
#include <c_tool.h>
#include <cl_erro_code.h>
#include <read_data.h>
#include <cl_gpu_profile_struct.h>
#include <cl_gpu_tool.h>

void init_test_particle_mem ( test_particle_profile_struct ** pPtr ){

    * pPtr = calloc ( 1 , sizeof ( * pPtr[0] ) ) ;
    
    test_particle_profile_struct * p = * pPtr ;
   
    read_points ( p ) ;

    #define mallocP(a) ( malloc( p->number * sizeof ( (a)[ 0 ] ) )) 
    
    //p->x = mallocP( p->x ) ; 
    p->v = mallocP( p->v ) ;
    p->F = mallocP( p->F ) ;
    p->m = mallocP( p->m ) ;
    p->q = mallocP( p->q ) ;
    
    #undef mallocP
}
void free_test_particle_mem ( test_particle_profile_struct * p ){
    iffree ( p->x );
    iffree ( p->v );
    iffree ( p->F ); 
    iffree ( p->m );
    iffree ( p->q ) ;
    iffree ( p ) ;
}
void cl_init_test_particle_mem ( test_particle_profile_struct * p , cl_gpu_profile_struct * g ){

    cl_int ret = 0 ;

    #define createBuffer(name){\
        p->cl_##name##_total_bytes = p->number * sizeof ( p->name [0] ) ;\
        p->cl_##name = clCreateBuffer(\
                    g->context,\
                    CL_MEM_READ_WRITE,\
                    p->cl_##name##_total_bytes ,\
                    NULL  ,\
                    &ret );\
    }

    createBuffer ( x ) ;   CL_CHECK ( ret );
    createBuffer ( v ) ;   CL_CHECK ( ret );
    createBuffer ( F ) ;   CL_CHECK ( ret );
    createBuffer ( m ) ;   CL_CHECK ( ret );
    createBuffer ( q ) ;   CL_CHECK ( ret );

    #undef createBuffer

    // no mem at host
    p->cl_cell_index_bytes = p->number * sizeof ( cell_index_type ) ;
    p->cl_cell_index = clCreateBuffer( g->context, CL_MEM_READ_WRITE, p->cl_cell_index_bytes , NULL , &ret ) ; CL_CHECK ( ret ) ;

}
void cl_free_test_particle_mem ( test_particle_profile_struct * p  ){

        
        cl_iffree ( p->cl_x ) ;
        cl_iffree ( p->cl_v ) ;
        cl_iffree ( p->cl_F ) ; 
        cl_iffree ( p->cl_m ) ; 
        cl_iffree ( p->cl_q ) ; 
        cl_iffree ( p->cl_cell_index ) ;

}

#endif