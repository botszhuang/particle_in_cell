#include <input.h>

int read_parameters( key_value_pair_struct * keys , char * inputFile ) {
    
    FILE * fptr = fopen( inputFile, "r" ) ; 
    if ( fptr == NULL) {
        printf("Error opening file(%s)!\n", inputFile);
        return EXIT_FAILURE;
    }
    
    const unsigned int buffer_size = 128 ;
    char * buffer = calloc( buffer_size , sizeof(char) ) ;
   
    char  *c1 , *c2 ;
    unsigned int len = 0 ;
   
    while ( fgets(buffer, buffer_size, fptr) != NULL) {
        for ( key_value_pair_struct * p = keys ; p->key != NULL ; p++ ) {
            if ( strncmp( buffer , p->key , strlen( p->key ) ) == 0 ){
                c1= strchr( buffer , '"' ) ;
                c2= strrchr( buffer , '"' ) ;
                len = c2 - c1 - 1 ;
                p->value = malloc(len + 1) ;
                strncpy( p->value , c1+1 , len ) ;
                p->value[ len ] = '\0' ;
        }
        }
    }
    fclose(fptr);
    free(buffer);

    return EXIT_SUCCESS ;
}

void print_parameters( key_value_pair_struct * keys ) {
    
    for ( key_value_pair_struct * p = keys ; p->key != NULL ; p++ ) {
        printf("\t%s: %s\n", p->key , p->value );
    }
}