#ifndef CELL_PRINT_NODE_C
#define CELL_PRINT_NODE_C

#include <stdio.h>
#include <cell_type.h>

void print_nodes_in_cell ( cell_profile_struct * c ) {

    #define X (c->node[i])

    for ( unsigned int i = 0 ; i < c->nodeSize ; i++ ) {
        puts ("node" );
        node_type_printf ( X , i ) ;            
    }

    #undef X

}

void print_cells ( cell_profile_struct * c ) {

    #define T (c->cell[ i ])

    for ( unsigned int i = 0 ; i < c->cellSize ; i++ ) {
        printf ( "cell "); 
        triangleStruct_printf ( T , i ) ;
    }

    #undef T
}
#endif