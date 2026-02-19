#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <cl_info.h>

#define printLine()   { printf("--------------------------\n"); }


void get_device_ids ( cl_platform_id platform_id , cl_device_id ** dev , unsigned int * dev_n ){

    cl_uint n = 0 ;

    clGetDeviceIDs( platform_id , CL_DEVICE_TYPE_GPU, 0, NULL, & n );

    if ( n < 0) {
        printf("  No devices found on this platform.\n");
        return ;
    }

    // Allocate memory and get the IDs
    * dev = malloc( n * sizeof(cl_device_id));
    clGetDeviceIDs( platform_id , CL_DEVICE_TYPE_GPU, n, *dev , NULL );

    * dev_n = n ;

}

unsigned int list_devices( cl_platform_id platform_id , bool print ) {

    cl_uint n = 0 ;
    cl_device_id * devices ;

    get_device_ids ( platform_id , & devices , & n ) ;

    if ( print ) {  printf("Device number: %i\n" , n ); }


    char name [ 128 ] ;

    for (cl_uint i = 0; i < n ; i++) {

        // Get device name and type
        clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(name), name, NULL);

        if ( print ) { printf("    %d: %s\n", i, name); }
    }

    free(devices);

    return n ;
}


void print_a_platform_info ( const cl_platform_id id ) {

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
    puts("");

    free ( buffer ) ;

#undef N
}

void print_all_platform_info (){


    cl_platform_id * list ;
    cl_uint n = 0 ;


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

        print_a_platform_info ( list [ i ] ) ;
        list_devices( list [ i ] , true ) ;
        printLine() ;


    }

    free ( list ) ;

}

void select_the_platform (  cl_platform_struct * g ) {

    cl_uint n = 0 ;
    cl_platform_id * list ;

    // list all platform ids
    clGetPlatformIDs( 0 , NULL , & n );

    if (n  == 0) {
        printf("No OpenCL platforms found!\n");
        exit(EXIT_FAILURE);
    }
    list = calloc( n , sizeof( list[0] ) );
    clGetPlatformIDs( n , list , NULL ) ;


    // select the specific platform
    char buffer[128];
    unsigned int buffer_size = sizeof ( buffer ) ;

    for ( unsigned int i = 0 ; i < n ; i++ ) {

        if ( ! list_devices( list [ i ] , false ) ) { continue ; }

        g->platform = list [ i ] ;
        break ;

    }

    free ( list ) ;

    print_a_platform_info ( g->platform ) ;

}
