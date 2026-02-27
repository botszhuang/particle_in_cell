__kernel void force_G( 
    __global double * G ,
    const unsigned int  N 
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
 
   double2 g = (double2) { 1 , 0 } ;

   for ( size_t i = gid ; i < N ; i += gtotal ) {

        vstore2 ( g , i , G )  ;
        //printf ("g[%zi]: =%lf %lf\n", i , g.x , g.y);
 
   }
   
} 
