#pragma once

#include <particle_struct.h>
#include <get_input_info.h>

void get_particle_memory ( particle_struct * particles , cl_context context ) ;
void free_particle_memory ( particle_struct * particles ) ;
void get_particle_profile ( particle_struct * particles , input_tex_tag_struct * input_tag ) ;
void write_particle_to_device ( particle_struct * particles , cl_command_queue queue , cl_event * write_event ) ;