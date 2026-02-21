#include <read_input_tex.h>
#include <grid.h>

double * relloacte_list_memory( double * list , unsigned int new_size ) {

    double * listtmp = realloc( list , new_size * sizeof( double ) ) ;

    if ( listtmp == NULL ) {
        printf("Error: Failed to reallocate memory for list.\n");
        return NULL ;
    }

    return listtmp ;

}

double * read_double ( size_t * out_size ,  char * inputFile ) {

    unsigned int counter = 0 ;

    FILE * fptr = fopen( inputFile, "r" ) ; 
    if ( fptr == NULL) {
        printf("Error opening file(%s)!\n", inputFile);
        return NULL ;
    }

    size_t listN = 10 ;
    double * list = calloc( listN , sizeof( double ) ) ;
    
    const unsigned int buffer_size = 128 ;
    char * buffer = calloc( buffer_size , sizeof(char) ) ;
    char * ptr ; 
    char * next_ptr;
    double val ;

    while ( fgets( buffer, buffer_size, fptr) != NULL) {
       
        // Skip comment lines
        if ( buffer[0] == '#' ) continue ; 

        next_ptr = NULL ;
        for ( ptr = buffer ; *ptr != '\n' ; ptr = next_ptr ) {

            val = strtod( ptr, &next_ptr ) ;

            // If no conversion happened
            if ( ptr == next_ptr ) {
                ptr++; 
                continue;
            }

            // 1. Dynamic Memory Check
            if (counter >= listN) {
                listN *= 2;
                list = relloacte_list_memory(list, listN); 
                if (list == NULL) break;
            }

            // 2. Store the value in the list
            list[counter] = val;
            counter++;
            
        }
    }

    free(buffer);
    fclose(fptr);

    if ( counter == 0 ) {       
        printf("No list read from the file,%s.\n", inputFile );
        free(list);
        return NULL ;
    } 

    * out_size = counter ;

    return list ;

}

int check_size_of_list( size_t list_size , unsigned int nD ) {
    if ( list_size % nD != 0 ) {
        printf("Error: Expected a multiple of %u values for coordinates, but got %zu.\n", nD, list_size);
        return EXIT_FAILURE ;
    }
    return EXIT_SUCCESS ;
}

#define dnD dimension_2D_struct
#define nD  2 
int read_2D ( dnD ** out , char * inputFile ) {

    size_t counter = 0 ;
   
    double * list = read_double ( &counter , inputFile ) ;

    if ( list == NULL ) { return 0 ; } 
    if ( check_size_of_list ( counter , nD ) != EXIT_SUCCESS ) { free(list); return 0 ; }

    const size_t num_points = counter / nD ;

    dnD * points = malloc( num_points * sizeof( points[0] ) ) ;
    if ( points == NULL ) {
        printf("Error: Failed to allocate memory for points.\n");
        free(list);
        return 0 ;
    }

    size_t index = 0 ;
    for ( size_t i = 0 ; i < num_points ; i++ ) {
        points[i].x = list[index  ];
        points[i].y = list[index+1];
        index += nD ;
    }

    free(list);

    * out = points ;

    return num_points ;
}
#undef nD
#undef dnD


void print_2D_list( dimension_2D_struct * list , size_t list_size ) {
    for ( size_t i = 0 ; i < list_size ; i++ ) {
        printf("%zu: (%.2f, %.2f)\n", i, list[i].x, list[i].y);
    }
}

