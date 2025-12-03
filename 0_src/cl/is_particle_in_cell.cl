//#include <c_and_cl_common.h>

inline bool point_in_triangle ( double2 A, double2 B, double2 C, double2 P ) {

    #define CROSS2(a,b) (((a).x)*((b).y) - (((a).y)*((b).x))) // z-component of 2D cross

    double2 v , p;

    v = B - A ;
    p = P - A ;
    double z1 = CROSS2( v , p );

    v = C - B ;
    p = P - B ;
    double z2 = CROSS2( v , p );

    v = A - C ;
    p = P - C ;
    double z3 = CROSS2( v , p );
   
    #undef CROSS2

    // Inside if all cross products have the same sign (or zero)
    // 1 -> true  -> P is inside the triangle
    // 0 -> false -> P is outside the triangle
    return ((z1 >= 0.0f && z2 >= 0.0f && z3 >= 0.0f) || // true
            (z1 <= 0.0f && z2 <= 0.0f && z3 <= 0.0f));  // false
            
}

__kernel void is_particle_in_cell(
    __global node_type *  nodePtr
  , unsigned int          nodeSize
  , __global triangleStruct  *  cellPtr
  , unsigned int                cellSize
  , __global x_dim *      particle
  , unsigned int          particleSize
  , __global cell_index_type * cell_index_of_p_Ptr
){
    const int gid0 = get_global_id(0);
    const int gid1 = get_global_id(1);

    const int gid0_size = get_global_size(0);
    const int gid1_size = get_global_size(1);
 
    triangleStruct triangle ;

    x_dim A , B , C ; // vortex of traingle
    x_dim P ; // point
    bool y ;
    for ( unsigned int triangleIndex = gid1 ; triangleIndex < cellSize ; triangleIndex += gid1_size  ) {

      triangle = cellPtr [ triangleIndex ] ;
      
      A = nodePtr [ triangle.x ] ;     
      B = nodePtr [ triangle.y ] ;
      C = nodePtr [ triangle.z ] ;

      for ( unsigned int particleIndex = gid0 ; particleIndex < particleSize ; particleIndex += gid0_size  ){

        P = particle [ particleIndex ] ;

        y = point_in_triangle ( A , B , C , P ) ;
        if ( y ) { 
          printf ( "p[%i] is inside cell[%i] ( %d %d %d )\n"  , particleIndex , triangleIndex  ) ; 
          cell_index_of_p_Ptr [ particleIndex ] = triangleIndex ;
        }
      }
    }

}