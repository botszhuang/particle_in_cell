#include <stdlib.h>
#include <stdio.h>

#define myfloat double

myfloat compute_a ( myfloat x ) {
    return 0.5 * ( 0 - x ) ;
}
myfloat compute_v_half ( const myfloat vIN, const myfloat aIN, const myfloat dt ){
    return ( vIN + ( aIN * dt ) ) ;
}

int main(){

    const myfloat x0 = -1 ;
    const myfloat v0 = 0.5 ;
    const myfloat dt = 0.1 ;
    const unsigned int N = 500 ;
    

    myfloat x = x0 ;
    myfloat v = v0 ;
    myfloat a = 0 ;


    puts ( "#x,a,v ") ;
    printf ( "%.3lf %.3lf %.3lf\n", x , a , v ) ;

    a = compute_a ( x ) ;
    v = compute_v_half ( v , a , ( dt * 0.5 ) );
    printf ( "#[0.5]%.3lf %.3lf %.3lf\n", x , a , v ) ;
    
    for ( unsigned int i = 0 ; i < N ; i++ ) {
        x += ( v * dt ) ;
        a  = compute_a ( x ) ;
        v  = compute_v_half ( v , a , dt ) ; 
        fprintf ( stdout, "%.3lf %.3lf %.3lf\n", x , a , v ) ; 
    }

    return EXIT_SUCCESS ;
}