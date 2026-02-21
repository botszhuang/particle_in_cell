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

void get_particle_memory ( particle_struct * particles , cl_context context ) {

    cl_int ret = 0 ;

    particles->cl_position_bytes     = particles->number * sizeof( particle_dimension ) ;
    particles->cl_velocity_bytes     = particles->number * sizeof( particle_dimension ) ;
    particles->cl_acceleration_bytes = particles->number * sizeof( particle_dimension ) ;

    particles->cl_position     = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_position_bytes     , NULL , &ret ) ; CL_CHECK( ret ) ;
    particles->cl_velocity     = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_velocity_bytes     , NULL , &ret ) ; CL_CHECK( ret ) ;
    particles->cl_acceleration = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_acceleration_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

}