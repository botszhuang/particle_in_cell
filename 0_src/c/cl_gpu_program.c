#ifndef CL_GPU_PROGRAM_C
#define CL_GPU_PROGRAM_C

#include <cl_gpu_profile_struct.h>
#include <cl_erro_code.h>

#include <stdio.h>

//$ nm cl_h/hello_world_cl.o | grep start
//$ nm cl_h/*.o | grep start
extern const char  _binary_cl_h_hello_world_cl_o_temp_null_start [] ;
extern const char  _binary_cl_h_c_and_cl_common_o_temp_null_start []  ;
extern const char  _binary_cl_h_leapfrog_step_cl_o_temp_null_start [] ;
extern const char  _binary_cl_h_is_particle_in_cell_cl_o_temp_null_start [] ;

void get_program ( cl_gpu_profile_struct * a ){

    //const char * source[] = { hello_world , leapfrog_step };

    const char *hello  = _binary_cl_h_hello_world_cl_o_temp_null_start ;
    const char *common = _binary_cl_h_c_and_cl_common_o_temp_null_start ;
    const char *leap   = _binary_cl_h_leapfrog_step_cl_o_temp_null_start ;
    const char *isPIC  = _binary_cl_h_is_particle_in_cell_cl_o_temp_null_start ;

    const char *sources[] = { common , hello , leap , isPIC }; // Common source first for includes
    int num_sources = sizeof( sources ) / sizeof(sources[0]);
    
    cl_int ret = 0 ;

    a->program = clCreateProgramWithSource( a->context, num_sources , sources, NULL , &ret);
    CL_CHECK ( ret ) ;
    
    ret = clBuildProgram( a->program, 1, a->device_Ids , NULL , NULL, NULL) ;
    print_cl_build_log( a->program , a->device_Ids[0] );
    CL_CHECK (ret) ;
   
}
void free_program ( cl_gpu_profile_struct *a ){ 
    if ( a->program ){ 
        CL_CHECK (clReleaseProgram(a->program));
    } 
}
#endif