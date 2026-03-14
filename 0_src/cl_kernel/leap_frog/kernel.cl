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
     //printf ( "v: %.3lf %.3lf ; x: %.3lf %.3lf\n", v.x , v.y , x.x , x.y ) ;  
     //printf ( "x:%.3lf %.3lf\n", x.x , x.y ) ;  

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
   myfloat2 v_old ;
   myfloat2 a_old ;

   for ( size_t i = gid ; i < N ; i += gtotal ) {

     a = vload2(i, A);
     v = vload2(i, V_half);
     v_old = vload2(i, V_half_out);
     a_old = vload2(i, A);

     // v = v + ( dt * a )
     //printf ( "old a: %.3lf %.3lf; old v: %.3lf %.3lf; v_out: %.3lf %.3lf\n", a_old.x , a_old.y , v_old.x , v_old.y , v.x , v.y ) ;  
     
     v = fma((myfloat2)(dt), a, v); 
     //printf ( "v: %.3lf %.3lf\n",v.x , v.y ) ;  
 
     vstore2(v, i, V_half_out);

   }
   
}
  
