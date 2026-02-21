#include <stdlib.h>
#include <cl_version.h>
#include <platform.h>

cl_event_struct initEventStruct( const unsigned int n) { 
    cl_event_struct ev = { .count = n }; 
    ev.array = malloc ( sizeof( ev.array [0] ) * ev.count ) ;
    return ev ; 
}

