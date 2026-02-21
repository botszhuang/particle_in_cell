#include <cl_version.h>
#include <cl_erro_code.h>
#include <platform.h>
#include <cl_kernel_list.h>

void run_kernel ( lauch_kernel_struct * k , 
                  cl_command_queue queue , 
                  cl_event_struct waitForEvent,
                  cl_event *event  ){

     CL_CHECK ( clEnqueueNDRangeKernel( queue,
                             k->kernel, 
                             k->work_dim, 
                             k->global_work_offset,
                             k->global_work_size,
                             k->local_work_size,
                             waitForEvent.count,
                             waitForEvent.array,
                             event ) ) ;
}

