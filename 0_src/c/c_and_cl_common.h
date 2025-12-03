#ifndef C_AND_CL_COMMON_H
#define C_AND_CL_COMMON_H

#define DIM 3

#if DIM == 3
    #ifdef __OPENCL_VERSION__   // Defined when compiling OpenCL kernels
        #define x_dim double2
        #define v_dim double3
        #define F_dim double3
    #else
        #include <CL/cl.h>  // Make sure cl_double2 is defined
        #define x_dim cl_double2
        #define v_dim cl_double3
        #define F_dim cl_double3
    #endif
#endif


#define t_type double
#define m_type double
#define q_type double
#define test_particle_number_type unsigned int
#define node_type x_dim
 
#define dt 0.1
#define lflf_printf(V,i)   printf( "[%i]\t%.3lf %.3lf\n"      , i , (V).x, (V).y )
#define lflflf_printf(V,i) printf( "[%i]\t%.3lf %.3lf %.3lf\n", i , (V).x, (V).y, (V).z )
#define ddd_printf(V,i)    printf( "[%i]\t%d %d %d\n"         , i , (V).x, (V).y, (V).z )

#define x_dim_printf(V,i) lflf_printf(V,i)
#define v_dim_printf(V,i) lflflf_printf(V,i)
#define F_dim_printf(V,i) lflflf_printf(V,i)

#define triangleStruct_printf(T,i) ddd_printf(T,i)
#define node_type_printf(N,i) lflf_printf(N,i)

#define cell_index_type unsigned int
typedef struct { unsigned int x ; unsigned int y ; unsigned int z ; } triangleStruct ;

#endif // c_and_cl_common.h