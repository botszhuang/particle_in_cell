__kernel void particle_pusher( 
    __global particle_dimension * position ,
    __global particle_dimension * velocity ,
    __global particle_dimension * force    ,
    unsigned int num_particles 
   ){
 
   const int gid = get_global_id (0) ;
   const int gtotal = get_global_size (0) ;
   const int N = num_particles ;

   for ( size_t i = 0 ; i < N ; i = i + gtotal ) {
        
   }
   
} 
