#include <get_input_info.h>
#include <stdio.h>
#include <string.h>

#define buffer_size 128
FILE * fptr ;
char * buffer ;

void init_read_input_tex( char * inputFile ) {
    fptr = fopen( inputFile, "r" ) ; 
    if ( fptr == NULL) {
        printf("Error opening file(%s)!\n", inputFile);
        exit(EXIT_FAILURE);
    }
    buffer = calloc( buffer_size , sizeof(char) ) ;
}
void close_read_input_tex() {
    free(buffer);
    fclose(fptr);
}
char * read_input_tex( char * key ) {
      
    char * value ;
    char  *c1 , *c2 ;
    unsigned int len = 0 ;
   
    while ( fgets(buffer, buffer_size, fptr) != NULL) {
        
        if ( strncmp( buffer , key , strlen( key ) ) != 0 ){ continue; }

        c1= strchr( buffer , '"' ) ;
        c2= strrchr( buffer , '"' ) ;
        len = c2 - c1 - 1 ;
        value = malloc(len + 1) ;
        strncpy( value , c1+1 , len ) ;
        value[ len ] = '\0' ;
        break ;
        
    }


    return value;
}

