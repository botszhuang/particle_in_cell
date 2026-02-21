
#include <cl_version.h>
#include <cl_erro_code.h>
#include <particle_struct.h>
#include <get_input_info.h>

void get_particle_memory ( particle_struct * particles , cl_context context ) {

    cl_int ret = 0 ;  

    particles->cl_position_bytes     = particles->number * sizeof( particle_dimension ) ;
    particles->cl_velocity_bytes     = particles->number * sizeof( particle_dimension ) ;
    particles->cl_acceleration_bytes = particles->number * sizeof( particle_dimension ) ;

    particles->cl_position     = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_position_bytes     , NULL , &ret ) ; CL_CHECK( ret ) ;
    particles->cl_velocity     = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_velocity_bytes     , NULL , &ret ) ; CL_CHECK( ret ) ;
    particles->cl_acceleration = clCreateBuffer( context , CL_MEM_READ_WRITE , particles->cl_acceleration_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

}
void free_particle_memory ( particle_struct * particles ) {
    CL_CHECK( clReleaseMemObject( particles->cl_position ) ) ;
    CL_CHECK( clReleaseMemObject( particles->cl_velocity ) ) ;
    CL_CHECK( clReleaseMemObject( particles->cl_acceleration ) ) ;
}
void get_particle_profile ( particle_struct * particles , input_tex_tag_struct * input_tag ) {
    particles->number = read_2D( & ( particles->position ) , input_tag->particle_position_file ) ;
    particles->number = read_2D( & ( particles->velocity ) , input_tag->particle_velocity_file ) ;
    //print_2D_list( particles->position , particles->number ) ;
    //print_2D_list( particles->velocity , particles->number ) ;
}
void write_particle_to_device ( particle_struct * particles , cl_command_queue queue , cl_event * write_event ) {
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, particles->cl_position,  asynchronous,    offset,  particles->cl_position_bytes, particles->position, 0, NULL, write_event   ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, particles->cl_velocity,  asynchronous,    offset,  particles->cl_velocity_bytes, particles->velocity, 0, NULL, write_event+1 ) ) ;
}