#include <string.h>
#include <stdio.h>

#include <cl_info.h>

#define printLine()   { printf("--------------------------\n"); }


static void print_info( const cl_platform_id id ) {

#define N 4096

    cl_platform_info param_ids[] = {
        CL_PLATFORM_NAME,
        CL_PLATFORM_VENDOR,
        CL_PLATFORM_VERSION,
        CL_PLATFORM_EXTENSIONS
    };

    const char * labels[] = { "Name", "Vendor", "Version", "Extensions" };
    const unsigned int labelsN = sizeof(labels) / sizeof(labels[0]);

    char * buffer = malloc ( N ) ;

    for ( unsigned int i = 0; i < labelsN ; i++) {

        clGetPlatformInfo( id , param_ids[i], N, buffer, NULL);
            printf("  %-12s: %s\n", labels[i], buffer);
            memset( buffer , 0,  N );
    }

    printLine() ;

    free ( buffer ) ;

#undef N
}

void print_all_plaform_info (){


    cl_platform_id * list ;
    cl_uint n ;


    clGetPlatformIDs( 0 , NULL , & n );

    if (n  == 0) {
        printf("No OpenCL platforms found!\n");
        exit(EXIT_FAILURE);
    }

    printf ( "platform number = %i\n" , n ) ;

    printLine() ;

    list = calloc( n , sizeof( list[0] ) );

    clGetPlatformIDs( n , list , NULL) ;

    for ( unsigned int i = 0 ; i < n ; i++ ) {

        print_info ( list [ i ] ) ;

    }

    free ( list ) ;

}

