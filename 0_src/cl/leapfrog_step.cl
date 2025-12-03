// leapfrog_step.cl
#define aFunc(i) (FPtr[i]*(qPtr[i]/mPtr[i])  )

__kernel void leapfrog_step0(
     __global x_dim * xPtr 
   , __global v_dim * vPtr 
   , __global F_dim * FPtr 
   , __global m_type * mPtr 
   , __global q_type * qPtr 
   ,          t_type tIn  // scale
   ,          test_particle_number_type pNum  // scale
     ) {
    
    const int gid = get_global_id(0);
    const test_particle_number_type g_id_size = get_global_size (0) ;

    x_dim x = 0 ;
    v_dim v = 0 ;
    F_dim a = 0 ;

    const t_type half_dt = 0.5 * dt ;

    for ( unsigned int i = gid ; i < pNum ; i += g_id_size ){
        
        x = xPtr [ i ] ;
        v = vPtr [ i ] ;
        a = aFunc(i) ;

        // Leapfrog step#1
        v += ( a * half_dt ) ; // half-step velocity
        //x += ( v * dt ) ;          // full-step position
        x.xy += ( v.xy * dt ) ; 
        xPtr [ i ] = x ;
        vPtr [ i ] = v ;

  
    /*printf("HELLO!\n") ;
    xPtr[i].x += 5 ; 
    printf("HELLO! %i, %.2lf %.2lf %.2lf,\t%.2lf %.2lf %.2lf,\t%.2lf %.2lf %.2lf,\t%.2lf %.2lf\n",
                  i, xPtr[i].x, xPtr[i].y, xPtr[i].z
                  , vPtr[i].x, vPtr[i].y, vPtr[i].z
                  , FPtr[i].x, FPtr[i].y, FPtr[i].z
                  , mPtr[i] , qPtr[i]
                  ) ;*/
    }
}

__kernel void leapfrog_step1(
     __global x_dim * xPtr 
   , __global v_dim * vPtr 
   , __global F_dim * FPtr 
   , __global m_type * mPtr 
   , __global q_type * qPtr 
   ,          t_type tIn  // scale
   ,          test_particle_number_type pNum  // scale
){
    const int gid = get_global_id(0);
    const test_particle_number_type g_id_size = get_global_size (0) ;

    v_dim v = 0 ;
    F_dim a = 0 ;
    const t_type half_dt = 0.5 * dt ;

    for ( unsigned int i = gid ; i < pNum ; i += g_id_size ){
        
        v = vPtr [ i ] ;
        a = aFunc(i) ;

        // Leapfrog step#2
        v += ( a * half_dt ) ; // half-step velocity
    
        vPtr [ i ] = v ;

  
    /*printf("HELLO!\n") ;
    xPtr[i].x += 5 ; 
    printf("HELLO! %i, %.2lf %.2lf %.2lf,\t%.2lf %.2lf %.2lf,\t%.2lf %.2lf %.2lf,\t%.2lf %.2lf\n",
                  i, xPtr[i].x, xPtr[i].y, xPtr[i].z
                  , vPtr[i].x, vPtr[i].y, vPtr[i].z
                  , FPtr[i].x, FPtr[i].y, FPtr[i].z
                  , mPtr[i] , qPtr[i]
                  ) ;*/
    }    
}
#undef aFunc
