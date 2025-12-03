#ifndef CL_ERROR_CODE_C
#define CL_ERROR_CODE_C

#include <cl_version.h>
#include <stdio.h>
#include <cl_erro_code.h>

#define CASE_RET(x) case x: return #x;

const char* clGetErrorString(cl_int err)
{
    switch (err)
    {
        // --- Run-time and compiler errors ---
        CASE_RET(CL_SUCCESS)
        CASE_RET(CL_DEVICE_NOT_FOUND)
        CASE_RET(CL_DEVICE_NOT_AVAILABLE)
        CASE_RET(CL_COMPILER_NOT_AVAILABLE)
        CASE_RET(CL_MEM_OBJECT_ALLOCATION_FAILURE)
        CASE_RET(CL_OUT_OF_RESOURCES)
        CASE_RET(CL_OUT_OF_HOST_MEMORY)
        CASE_RET(CL_PROFILING_INFO_NOT_AVAILABLE)
        CASE_RET(CL_MEM_COPY_OVERLAP)
        CASE_RET(CL_IMAGE_FORMAT_MISMATCH)
        CASE_RET(CL_IMAGE_FORMAT_NOT_SUPPORTED)
        CASE_RET(CL_BUILD_PROGRAM_FAILURE)
        CASE_RET(CL_MAP_FAILURE)
        CASE_RET(CL_MISALIGNED_SUB_BUFFER_OFFSET)
        CASE_RET(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)
        CASE_RET(CL_COMPILE_PROGRAM_FAILURE)
        CASE_RET(CL_LINKER_NOT_AVAILABLE)
        CASE_RET(CL_LINK_PROGRAM_FAILURE)
        CASE_RET(CL_DEVICE_PARTITION_FAILED)
        CASE_RET(CL_KERNEL_ARG_INFO_NOT_AVAILABLE)

        // --- Compile-time errors ---
        CASE_RET(CL_INVALID_VALUE)
        CASE_RET(CL_INVALID_DEVICE_TYPE)
        CASE_RET(CL_INVALID_PLATFORM)
        CASE_RET(CL_INVALID_DEVICE)
        CASE_RET(CL_INVALID_CONTEXT)
        CASE_RET(CL_INVALID_QUEUE_PROPERTIES)
        CASE_RET(CL_INVALID_COMMAND_QUEUE)
        CASE_RET(CL_INVALID_HOST_PTR)
        CASE_RET(CL_INVALID_MEM_OBJECT)
        CASE_RET(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
        CASE_RET(CL_INVALID_IMAGE_SIZE)
        CASE_RET(CL_INVALID_SAMPLER)
        CASE_RET(CL_INVALID_BINARY)
        CASE_RET(CL_INVALID_BUILD_OPTIONS)
        CASE_RET(CL_INVALID_PROGRAM)
        CASE_RET(CL_INVALID_PROGRAM_EXECUTABLE)
        CASE_RET(CL_INVALID_KERNEL_NAME)
        CASE_RET(CL_INVALID_KERNEL_DEFINITION)
        CASE_RET(CL_INVALID_KERNEL)
        CASE_RET(CL_INVALID_ARG_INDEX)
        CASE_RET(CL_INVALID_ARG_VALUE)
        CASE_RET(CL_INVALID_ARG_SIZE)
        CASE_RET(CL_INVALID_KERNEL_ARGS)
        CASE_RET(CL_INVALID_WORK_DIMENSION)
        CASE_RET(CL_INVALID_WORK_GROUP_SIZE)
        CASE_RET(CL_INVALID_WORK_ITEM_SIZE)
        CASE_RET(CL_INVALID_GLOBAL_OFFSET)
        CASE_RET(CL_INVALID_EVENT_WAIT_LIST)
        CASE_RET(CL_INVALID_EVENT)
        CASE_RET(CL_INVALID_OPERATION)
        CASE_RET(CL_INVALID_GL_OBJECT)
        CASE_RET(CL_INVALID_BUFFER_SIZE)
        CASE_RET(CL_INVALID_MIP_LEVEL)
        CASE_RET(CL_INVALID_GLOBAL_WORK_SIZE)
        CASE_RET(CL_INVALID_PROPERTY)

        // --- OpenCL 2.x / 3.x valid error codes ---
#if defined(CL_VERSION_2_0) || defined(CL_VERSION_3_0)
        CASE_RET(CL_INVALID_IMAGE_DESCRIPTOR)
        CASE_RET(CL_INVALID_COMPILER_OPTIONS)
        CASE_RET(CL_INVALID_LINKER_OPTIONS)
        CASE_RET(CL_INVALID_DEVICE_PARTITION_COUNT)
#endif

        default:
            return "Unknown OpenCL error";
    }
}

void print_cl_build_log( cl_program program, cl_device_id device )
{
    size_t size = 0;
    clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);

    if (!size) {
        printf("<empty build log>\n");
        return;
    }

    char *log = malloc(size + 1);
    clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, size, log, NULL);
    log[size] = '\0';

    printf("\n===== OpenCL Build Log =====\n%s\n", log);
    printf("===== End Build Log ========\n\n");

    free(log);
}


#endif