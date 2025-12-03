#ifndef C_DEFINE_H
#define C_DEFINE_H

#include <cl_version.h>
#include <c_and_cl_common.h>

typedef struct{
  
  test_particle_number_type number ;  

  x_dim * x ;
  v_dim * v ;
  F_dim * F ;

  m_type * m ; // effective mass
  q_type * q ; // effective charge

  cl_mem cl_x ;
  cl_mem cl_v ;
  cl_mem cl_F ;
  cl_mem cl_m ;
  cl_mem cl_q ;
  cl_mem cl_cell_index ;

  unsigned int cl_x_total_bytes ;
  unsigned int cl_v_total_bytes ;
  unsigned int cl_F_total_bytes ;
  unsigned int cl_m_total_bytes ;
  unsigned int cl_q_total_bytes ;
  unsigned int cl_cell_index_bytes ;

} test_particle_profile_struct ;



#endif