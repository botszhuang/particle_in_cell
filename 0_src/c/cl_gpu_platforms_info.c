#ifndef CL_GET_PLATFORMS_INFO_C
#define CL_GET_PLATFORMS_INFO_C

#include <cl_gpu_profile_struct.h>
#include <cl_erro_code.h>
#include <c_tool.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static cl_device_id * get_devices_info(unsigned int *device_numPtr, cl_platform_id platform) {

    cl_int ret = 0;
    cl_uint num_devices = 0;

    // Query GPU count
    ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
    if (ret == CL_DEVICE_NOT_FOUND || num_devices == 0) {
        printf("\tNo GPU devices on this platform.\n");
        *device_numPtr = 0;
        return NULL;
    } 
    else if (ret != CL_SUCCESS) {
        fprintf(stderr, "\tError getting GPU devices: %d\n", ret);
        *device_numPtr = 0;
        return NULL;
    }

    *device_numPtr = num_devices;

    // Allocate device list
    cl_device_id *devices = malloc(sizeof(cl_device_id) * num_devices);
    if (!devices) {
        fprintf(stderr, "\tMemory allocation failed.\n");
        *device_numPtr = 0;
        return NULL;
    }

    // Get device IDs
    ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "\tError getting GPU devices: %d\n", ret);
        free(devices);
        *device_numPtr = 0;
        return NULL;
    }

    // Print device info
    printf("\tFound %u GPU device(s):\n", num_devices);
    char name[128];
    cl_bool available = false;
    for (cl_uint j = 0; j < num_devices; j++) {
        clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 128, name, NULL);
        clGetDeviceInfo(devices[j], CL_DEVICE_AVAILABLE, sizeof(cl_bool), &available, NULL);

        printf("  GPU Device %u: %s (Available: %s)\n", j, name, available ? "Yes" : "No");
    }

    return devices;
}
static void get_platform_info(const cl_platform_id *id ) {
    
    char pname   [128];
    char vendor  [128];      
    char version [128];
    char extensions [1024];

    clGetPlatformInfo( * id, CL_PLATFORM_NAME, 128, pname, NULL);
    clGetPlatformInfo( * id, CL_PLATFORM_VENDOR, 128, vendor, NULL);
    clGetPlatformInfo( * id, CL_PLATFORM_VERSION, 128, version, NULL);
    clGetPlatformInfo( * id, CL_PLATFORM_EXTENSIONS, 1024, extensions, NULL);

    printf("  Name:      %s\n", pname);
    printf("  Vendor:    %s\n", vendor);
    printf("  Version:   %s\n", version);
    printf("  Extensions:\n    %s\n", extensions);
  
}

void get_platform_number_and_ids(cl_gpu_profile_struct *g) {

    cl_uint num_platforms = 0;
    cl_platform_id * pidPtr ;
    cl_platform_id   pid    ;
    unsigned int p_count = 0 ;

    // get platform count
    CL_CHECK(clGetPlatformIDs(0, NULL, &p_count));
    if (p_count == 0) {
        printf("No OpenCL platforms found!\n");
        exit(EXIT_FAILURE);
    }

    pidPtr = calloc(p_count, sizeof(cl_platform_id));
    CL_CHECK(clGetPlatformIDs(p_count, pidPtr, NULL));

    printf("Found %u platform(s)\n", p_count);
    for (unsigned int i = 0; i < p_count; i++) {
        printf("\nPlatform %u : \n", i);
        get_platform_info( pidPtr +i );
    }

    char pname[128];
    for (unsigned int i = 0; i < p_count; i++) {
        pid = pidPtr [ i ] ;
        clGetPlatformInfo( pid, CL_PLATFORM_NAME, 128, pname, NULL);

        if ( ! strcmp("NVIDIA CUDA", pname ) ) {
            printf("\n#-------- Using platform %u : %s ---------\n", i, pname);
            get_platform_info( pidPtr +i );
            g->device_Ids = get_devices_info(&g->device_number, pid );
            if ( g->device_Ids == NULL || g->device_number == 0 ) {
                printf("No GPU devices found on platform %s\n", pname);
                exit(EXIT_FAILURE);
            }
            g->platform_id_num = p_count ;
            g->platform_Ids    = pid ;
            printf("#------------------------------------------\n");
            break ;
        }
    }
}


void free_device(cl_gpu_profile_struct *g) {
    for (unsigned int i = 0; i < g->device_number ; i++) {
        iffree(g->device_Ids [i]);
    }
}

void free_platform(cl_gpu_profile_struct *g) {
    iffree(g->platform_Ids);
}

#endif
