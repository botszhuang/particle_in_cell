#ifndef CELL_TYPE_H
#define CELL_TYPE_H

#include <cl_version.h>
#include <c_and_cl_common.h>

typedef struct{
  
  unsigned int    cellSize ;  
  triangleStruct * cell  ;

  unsigned int nodeSize ;
  node_type * node ;

  cl_mem cl_node ;
  cl_mem cl_cell ;

  unsigned int cl_node_total_bytes ;
  unsigned int cl_cell_total_bytes ;
  

  } cell_profile_struct ;

#endif