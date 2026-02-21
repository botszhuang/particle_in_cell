#pragma once

#include <stdlib.h>
#include <dimension.h>

void init_read_input_tex( char * inputFile );
void close_read_input_tex() ;
char * read_input_tex( char * key ) ;

int read_2D ( dimension_2D_struct ** out , char * inputFile ) ;
void print_2D_list( dimension_2D_struct * list , size_t list_size ) ;
