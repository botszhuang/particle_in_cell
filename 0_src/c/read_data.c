#ifndef CELL_READ_NODE_C
#define CELL_READ_NODE_C

#include <stdio.h>
#include <cl_erro_code.h>
#include <c_tool.h>
#include <cl_gpu_profile_struct.h>
#include <read_data.h>

#define fPointFile  "points.csv"
#define fNodeFile  "nodes.txt"
#define fCellFile  "elements.txt"

#define checkCellNode(a) {\
    if (!a) {\
        printf("realloc failed for ##a @ %s %d", __FILE__, __LINE__ );\
        iffree(a);\
        iffree(c);\
        exit(EXIT_FAILURE);\
    }\
}

static inline void examfopen(FILE *fp, const char *fn){
        if (!fp) {
        printf("Error opening file %s at func:%s at %s, line %d\n", fn, __func__ , __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}
static inline void skipCSVhead( FILE * fp ) {
    char header[200];
    fgets(header, sizeof(header), fp);
}
static inline void rellocation_func ( void ** p , size_t pSize, const size_t counter ,  const size_t element_size ) {
    if ( counter == pSize ) {
        pSize += 100 ;
        void * tmp = realloc( *p, ( pSize * element_size ) );
        if ( ! tmp ) {\
            printf ( "realloc failed %s, %d" , __FILE__ , __LINE__  );
            iffree(p);  
            //fclose(fp);
            exit(EXIT_FAILURE);
    }
    p = tmp ;    
    }
}

static inline void read_x_dim_CSV ( const char * filename , x_dim ** lflf_array , unsigned int * lflf_size ) {

    printf ( "reading %s ...\n", filename ) ;

    FILE * fp = fopen( filename , "r");
    examfopen( fp , filename ) ;
    skipCSVhead( fp );

    // reading CSV file
    x_dim X ;
    const unsigned int sizeofElement = sizeof( X ) ;
    unsigned int ListSize = 30 ;
    x_dim * List0 = malloc ( ListSize * sizeofElement  )  ;
    unsigned int counter = 0  ;
    
    while ( fscanf ( fp , "%lf,%lf", &X.x, &X.y ) == 2) {
        List0 [ counter ] = X ;
        counter ++ ;
        rellocation_func( (void **)&List0 , ListSize , counter , sizeofElement ) ;
    }
    // end of reading CSV file

    fclose ( fp );

    // update output
    *lflf_size  = counter ;
    *lflf_array = realloc ( List0 , counter * sizeofElement ) ;

}
static inline void read_3int_CSV  ( const char * filename , int ** int3_array   , unsigned int * int3_size ) {

    printf ( "reading %s ...\n", filename ) ;

    FILE * fp = fopen( filename , "r");
    examfopen( fp , filename ) ;
    skipCSVhead( fp );  

    // reading CSV file
    triangleStruct X ;
    const unsigned int sizeofElement = sizeof( X ) ;
    unsigned int ListSize = 30 ;
    triangleStruct * List0 = malloc ( ListSize * sizeofElement  )  ;
    unsigned int counter = 0  ;
    
    while ( fscanf ( fp , "%d,%d,%d", &X.x, &X.y, &X.z ) == 3) {
        List0 [ counter ] = X ;
        counter ++ ;
        rellocation_func( (void **)&List0 , ListSize , counter , sizeofElement ) ;
    }
    // end of reading CSV file

    fclose ( fp );

    // update output
    *int3_size  = counter ;
    *int3_array = realloc ( List0 , counter * sizeofElement ) ;

}

void read_points ( test_particle_profile_struct * c ) {
  
     
    read_x_dim_CSV ( fPointFile , &( c->x ) , &( c->number ) ) ;
    checkCellNode(c->x) ;

}
void read_node ( cell_profile_struct * c ) {

    read_x_dim_CSV ( fNodeFile , &( c->node ) , &( c->nodeSize ) ) ;
    checkCellNode(c->node) ;

}
void read_cell ( cell_profile_struct * c ) {

    read_3int_CSV ( fCellFile , (int **)&( c->cell ) , &( c->cellSize ) ) ;
    checkCellNode(c->cell);  // Check if reallocation succeeded

}

#undef fNodeFile
#undef fCellFile
#undef fPointFile

#endif