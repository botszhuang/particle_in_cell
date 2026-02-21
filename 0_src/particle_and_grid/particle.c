#include <cl_erro_code.h>
#include <read_input_tex.h>
#include <get_input_info.h>
#include <grid.h>
#include <particle.h>

void get_particle_profile ( particle_struct * particles ,  input_tex_tag_struct * input_tag ) {

    particles->number = read_2D( & ( particles->position ) , input_tag->particle_position_file ) ;
    particles->number = read_2D( & ( particles->velocity ) , input_tag->particle_velocity_file ) ;

    print_2D_list( particles->position , particles->number ) ;
    print_2D_list( particles->velocity , particles->number ) ;
}

