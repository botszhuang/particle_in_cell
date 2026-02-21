#pragma once
#include <cl_version.h>
#include <dimension.h>
#include <get_input_info.h>
#include <particle_struct.h>

void get_particle_profile ( particle_struct * particles ,  input_tex_tag_struct * input_tag ) ;
void get_particle_memory ( particle_struct * particles , cl_context context ) ;