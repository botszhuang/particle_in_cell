// step 0 :
// x = x0
// a = A(x) compute A
// v_half = v0 + ( 0.5 * dt * a )
// loop: 1 --> 3
// step1: x = x + ( dt * v_half )
// step2: a = A(x)  compute A
// step3: v_half = v_half + ( dt * a ) 
__kernel void leap_frog_X( 
    __global double2 * X ,
    __global double2 * V_half ,
    const unsigned int  N  ,
    const myfloat dt
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;

   printf ("x: dt=%lf\n",dt);
   
   for ( size_t i = 0 ; i < N ; i = i + gtotal ) {
        
        X[i] += ( dt * V_half[i] ) ;
 
   }
   
} 
__kernel void leap_frog_V_half( 
    __global double2 * V_half ,
    __global double2 * A ,
    const unsigned int  N  ,
    const myfloat dt
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;

   printf ("v: dt=%lf\n",dt);

   for ( size_t i = 0 ; i < N ; i = i + gtotal ) {
        
        V_half[i] += ( dt * A[i] ) ;
 
   }
   
} 
