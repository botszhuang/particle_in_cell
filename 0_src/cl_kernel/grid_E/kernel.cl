__kernel void grid_E( 
    const unsigned int grid_number ,
    __global myfloat * grid_X ,
    __global myfloat * grid_E ,
    const unsigned int  particle_number ,
    __global myfloat * particle_X 
){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
 
   myfloat2 x ;
   myfloat2 g ;
   myfloat2 e ;

   myfloat l ;
   myfloat2 dx ;
   myfloat lMax = 5*5 ;
   myfloat lMin = 0.001 * 0.001 ;
 
   for ( size_t i = gid ; i < particle_number ; i += gtotal ) {

     x = vload2(i, particle_X );

     g.x = 0.5 * ( 0 - x.x ) ;
     g.y = 0 ; 
     for ( unsigned int j = 0 ; j < grid_number ; ++j ){

      g = vload2 ( j, grid_X ) ;
      l = distance ( x , g ) ;
      if ( l > lMax ) { continue ; }
      if ( l < lMin ) { continue ; }
      printf ( "gid = %i, x[%i]:(%.3lf,%.3lf), g[%i]:(%.3lf,%.3lf), l=%.3lf\n", gid , i , x.x , x.y, j , g.x, g.y , l ) ;
     }
     vstore2 ( e , i , grid_E )  ;
 
   }
   
} 
