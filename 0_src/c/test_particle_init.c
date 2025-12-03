#ifndef TEST_PARTICLE_INIT_C
#define TEST_PARTICLE_INIT_C
#include <c_tool.h>
#include <test_particle_type.h>

void init_test_particle_status ( test_particle_profile_struct * p ){

    const typeof( (p->number) ) num = p->number ;

    #define R ( (double) rand()/ (RAND_MAX +1.0))

    for ( unsigned int i = 0 ; i < num ; i++ ) {
        //p->x[i] = ( x_dim ) { 2*R,  2*R } ;
        p->v[i] = ( v_dim ) { 20, 21, 22 } ;
        p->F[i] = ( F_dim ) { 30, 31, 32 } ;
        p->m[i] = 1 ;
        p->q[i] = 1 ;
    }

    #undef R
}

#endif