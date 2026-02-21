#include <read_input_tex.h>
#include <get_input_info.h>

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ){
    printf("### Reading %s ......\n" , inputFile ) ;
    init_read_input_tex( inputFile ) ;
    input_tag->grid_file = read_input_tex( "grid_file" ) ;
    input_tag->particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag->particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;
}