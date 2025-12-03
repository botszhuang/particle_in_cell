#ifndef TEST_PARTICLE_RUN_H
#define TEST_PARTICLE_RUN_H

#include <cl_gpu_profile_struct.h>
#include <test_particle_type.h>

void init_test_particle_mem ( test_particle_profile_struct ** pPtr ) ;
void free_test_particle_mem ( test_particle_profile_struct * p ) ;

void init_test_particle_status ( test_particle_profile_struct * p );
void print_test_particle ( test_particle_profile_struct * p );

void cl_init_test_particle_mem ( test_particle_profile_struct * p , cl_gpu_profile_struct * g );
void cl_free_test_particle_mem ( test_particle_profile_struct * p  );

#define host_to_gpu(p, g, name) \
    CL_CHECK( clEnqueueWriteBuffer( g->queue, (p)->cl_##name, \
                                    CL_TRUE, 0, \
                                    (p)->cl_##name##_total_bytes, \
                                    (p)->name, 0, NULL, NULL ) )

#define gpu_to_host(p, g, name) \
    CL_CHECK( clEnqueueReadBuffer( g->queue, p->cl_##name, \
                                   CL_TRUE, \
                                   0, \
                                   p->cl_##name##_total_bytes, \
                                   p->name, \
                                   0, NULL, NULL ) )

#endif