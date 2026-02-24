__kernel void force_G( 
    __global double2 * G ,
    const unsigned int  N 
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
 
   for ( size_t i = 0 ; i < N ; i = i + gtotal ) {

        G[i] = ( double2 ) { 0 , 1.0 }  ;
        printf ("g[%zi]: =%lf %lf\n", i , G[i].x , G[i].y);
 
   }
   
} 
