__kernel void particle_pusher( 
    __global particle_dimension * position ,
    __global particle_dimension * velocity ,
    __global particle_dimension * force    ,
    const unsigned int  N  ,
    const myfloat dt
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
   
   particle_dimension X ;
   particle_dimension V ;
   particle_dimension F ;

   for ( size_t i = 0 ; i < N ; i = i + gtotal ) {
        X = position [ i ] ;
        V = velocity [ i ] ;
        F = force    [ i ] ;

   }
   
} 
