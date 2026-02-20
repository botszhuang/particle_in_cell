#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char * key ;    
    char * value ;
}key_value_pair_struct ;

int read_parameters( key_value_pair_struct * keys , char * inputFile ) ;
void print_parameters( key_value_pair_struct * keys ) ;
key_value_pair_struct * set_key( ) ;
