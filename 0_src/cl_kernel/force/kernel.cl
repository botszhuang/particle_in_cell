__kernel void force_G( 
    __global myfloat * G ,
    __global myfloat * X ,
    const unsigned int  N ,
    __global myfloat * GOUT 
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
 
   myfloat2 g = (myfloat2) { 1 , 0 } ;
   //myfloat2 k = (myfloat2) { 1 , 0 } ;
   myfloat  k = 1.0 ;
   myfloat2 x = (myfloat2) { 0 , 0 } ;

   //printf("from force\n"); 
   for ( size_t i = gid ; i < N ; i += gtotal ) {

     vstore2 ( x , i , X )  ;
     g.x = k *  ( 5 - x.x ) ;
     g.y = 0 ; 
     //printf ("g[%zi]: =%lf %lf\n", i , g.x , g.y);
     vstore2 ( g , i , GOUT )  ;
 
   }
   
} 
