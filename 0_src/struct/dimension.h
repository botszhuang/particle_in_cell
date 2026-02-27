#ifndef DIMENSION_H
#define DIMENSION_H

#ifdef __OPENCL_VERSION__
    #pragma OPENCL EXTENSION cl_khr_fp64 : enable
    #define ALIGN(x) 
#else
    // 2. Handle Host (C/C++) alignment
    #if defined(_MSC_VER)
        #define ALIGN(x) __declspec(align(x)) // For Visual Studio
    #else
        #define ALIGN(x) __attribute__((aligned(x))) // For GCC/Clang
    #endif
#endif

#define myfloat  double
#define myfloat2 double2

typedef struct ALIGN(16){
    myfloat x ;
    myfloat y ;
} dimension_2D_struct ;

#define particle_dimension dimension_2D_struct
#define grid_dimension     dimension_2D_struct

#endif

