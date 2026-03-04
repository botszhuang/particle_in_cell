// step 0 :
// x = x0
// a = A(x) compute A
// v_half = v0 + ( 0.5 * dt * a )
// loop: 1 --> 3
// step1: x = x + ( dt * v_half )
// step2: a = A(x)  compute A
// step3: v_half = v_half + ( dt * a ) 
__kernel void leap_frog_X( 
    __global myfloat * X ,
    __global myfloat * V_half ,
    __global myfloat * X_out  ,
    const unsigned int  N  ,
    const myfloat dt
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;

   //printf ("x: dt=%lf\n",dt*3*gid);

   myfloat2 x ;
   myfloat2 v ;

   for ( size_t i = gid ; i < N ; i += gtotal ) {
        
     x = vload2(i, X);
     v = vload2(i, V_half);

     // x = x + ( dt * v )
     x = fma((myfloat2)(dt), v, x); //Fused Multiply-Add

     vstore2(x, i, X_out);
 
   }
   
} 
__kernel void leap_frog_V_half( 
    __global myfloat * V_half ,
    __global myfloat * A ,
    __global myfloat * V_half_out ,
    const unsigned int  N  ,
    const myfloat dt
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;

   //printf ("v: dt=%lf\n",dt*gid);

   myfloat2 a ;
   myfloat2 v ;

   for ( size_t i = gid ; i < N ; i += gtotal ) {

     a = vload2(i, A);
     v = vload2(i, V_half);

     // v = v + ( dt * a )
     v = fma((myfloat2)(dt), a, v);   
 
     vstore2(v, i, V_half_out);

   }
   
}
  
