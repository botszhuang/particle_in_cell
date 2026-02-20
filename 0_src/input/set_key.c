#include <input.h>

#define keyStrct key_value_pair_struct

keyStrct * set_key( ) {

    unsigned int num_keys = 3 ;
    keyStrct * keys = ( keyStrct * ) malloc( num_keys * sizeof( keyStrct ) ) ;
    
    keys[0] = (keyStrct) { .key = "grid_file" };
    keys[1] = (keyStrct) { .key = "particle_file" };
    keys[2] = (keyStrct) { .key = NULL };
    
    return keys ;  
}

#undef keyStrct