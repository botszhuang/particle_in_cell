__kernel void force( 
    const unsigned int  GN ,
    __global myfloat * G ,
    const unsigned int  XN ,
    __global myfloat * X ,
    __global myfloat * F ,
    __global myfloat * FOUT 
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
 
   myfloat2 f ;
   myfloat2 x ;
   myfloat2 g ;
 
   for ( size_t i = gid ; i < XN ; i += gtotal ) {

     x = vload2(i, X );

     f.x = 0.5 * ( 0 - x.x ) ;
     f.y = 0 ; 
     vstore2 ( f , i , FOUT )  ;
 
   }
   
} 
