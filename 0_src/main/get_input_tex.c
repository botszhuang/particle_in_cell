#include <main_struct.h>

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ){
    printf("### Reading %s ......\n" , inputFile ) ;
    init_read_input_tex( inputFile ) ;
    input_tag->grid_file = read_input_tex( "grid_file" ) ;
    input_tag->particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag->particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;
}

void freeEventArray( const unsigned int N , cl_event * eventArray ){
    for (unsigned int j = 0; j < N; j++) {
        clReleaseEvent( eventArray [ j ] );         
    }
}


