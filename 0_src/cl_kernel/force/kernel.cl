__kernel void force_G( 
    __global myfloat * G ,
    __global myfloat * X ,
    const unsigned int  N ,
    __global myfloat * GOUT 
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
 
   myfloat2 g ;
   myfloat2 x ;

   //printf("from force\n"); 
   for ( size_t i = gid ; i < N ; i += gtotal ) {

     x = vload2(i, X );
     g.x = 0.5 * ( 0 - x.x ) ;
     g.y = 0 ; 
     printf ("a:%.3lf %.3lf\n", g.x , g.y);
     vstore2 ( g , i , GOUT )  ;
 
   }
   
} 
