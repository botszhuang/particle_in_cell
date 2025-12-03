#ifndef CELL_CL_MEM_C
#define CELL_CL_MEM_C

#include <stdio.h>
#include <cl_erro_code.h>
#include <c_tool.h>
#include <cell_type.h>
#include <cl_gpu_profile_struct.h>
#include <cl_gpu_tool.h>


void cl_cell_mem_init ( cell_profile_struct * c , 
                        cl_gpu_profile_struct * g ){

    cl_int ret = 0 ;

    c->cl_node_total_bytes = c->nodeSize * sizeof ( c->node [0] ) ;
    c->cl_cell_total_bytes = c->cellSize * sizeof ( c->cell [0] ) ;

    #define createBuffer(name){ \
        c->cl_##name = clCreateBuffer(\
                    g->context,\
                    CL_MEM_READ_WRITE,\
                    c->cl_##name##_total_bytes ,\
                    NULL  ,\
                    &ret );\
    }

    createBuffer ( node ) ;   CL_CHECK ( ret );
    createBuffer ( cell ) ;   CL_CHECK ( ret );
   
    #undef createBuffer
}

void free_cell_profile ( cell_profile_struct *c ) { 
    iffree ( c->node ) ;
    iffree ( c->cell ) ; 
    cl_iffree ( c->cl_node ) ;
    cl_iffree ( c->cl_cell ) ;
    iffree ( c ) ;
}

#endif