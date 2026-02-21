#include <platform.h>
#include <get_input_info.h>
#include <cl_kernel_list.h>

#define printLine()   { printf("-----------------------\n"); }

typedef struct {
    char * grid_file ;
    char * particle_position_file ;
    char * particle_velocity_file ;   
} input_tex_tag_struct ;
typedef struct {
    cl_mem X ;
    cl_mem V ;
    cl_mem F ;
    size_t X_bytes ;
    size_t V_bytes ;
    size_t F_bytes    ;
} particle_cl_mem_struct ;
typedef struct {
    cl_mem X ;
    size_t X_bytes ;
} grid_cl_mem_struct;
typedef struct{
    size_t number ;
    particle_dimension * X ;
    particle_dimension * V ;
    particle_dimension * F ;
} particle_struct ;
typedef struct{
    size_t number;
    grid_dimension * X ;
} grid_struct ;
typedef struct {
   particle_struct particle ;
   particle_cl_mem_struct particle_cl_mem ;
   grid_struct grid ;
   grid_cl_mem_struct grid_cl_mem ;
}data_struct;

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile );
void get_data_profile     ( data_struct * data , input_tex_tag_struct * tag );
void create_dev_memory    ( data_struct * data , platform_struct * g ) ;
void free_dev_memory      ( data_struct * data ) ;
void write_data_to_device ( data_struct *data , cl_command_queue queue , cl_event * write_event ) ;

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
    printLine() ; 
    printf("Hello world!\n") ;

    // initialize the platform
    print_all_platform_info () ;
    platform_struct gpu ;
    cl_kernel hello_world_kernel ;

    select_the_platform ( &gpu ) ;
    get_device_ids ( gpu.platform , & gpu.devices , & gpu.device_N ) ;
    create_context ( &gpu );
    create_queue   ( &gpu );
    init_program ( &gpu );

    puts("### Running hello world kernel ......") ;
    create_hello_world_kernel ( &hello_world_kernel , gpu.program ) ;
    run_kernel_hello_world ( hello_world_kernel , gpu.queue[0] ) ;

    // events' parameters
    const cl_int write_grid_count     = 1 ;
    const cl_int write_particle_count = 2 ;
    cl_event_struct write_events = initEventStruct ( write_grid_count + write_particle_count ) ;
    cl_event_struct particle_pusher_events = initEventStruct( 1 ) ;
    
    lauch_kernel_struct particle_pusher ;

    cl_uint work_dim = 1 ;
    size_t global_work_offset[1] = {0} ;
    size_t global_work_size[1] = {1} ;
    size_t local_work_size[1] = {1} ;

    puts("### Creating particle pusher kernel ......") ;
    create_particle_pusher_kernel ( & particle_pusher , gpu , work_dim , global_work_offset , global_work_size ,  local_work_size  ) ;
 
    
    // read input.tex
    input_tex_tag_struct input_tag ;
    get_input_tex ( &input_tag , inputFile ) ;

    data_struct data;
 
    puts("### Reading grid and particle profiles ......") ;
    get_data_profile ( &data , &input_tag ) ;

    puts("### Allocating memory for grid and particle on the device ......") ;
    create_dev_memory ( &data , &gpu ) ;

    puts("### Setting the arguments for kernel: particle pushe ......") ;
    set_particle_pusher_kernel_args ( & particle_pusher , 
                                    &( data.particle_cl_mem.X ) , 
                                    &( data.particle_cl_mem.V ) ,
                                    &( data.particle_cl_mem.F ) ,
                                    &  data.particle.number   ) ;

    puts("### Writing grid and particle data to the device ......") ;
    write_data_to_device     ( &data  , gpu.queue[0] , write_events.array ) ;
    //waitForEvents ( write_events ) ;
    //releaseEventArray ( write_events ) ;



    puts("### Running particle pusher kernel ......") ;
    run_kernel ( & particle_pusher , gpu.queue[0] , write_events , particle_pusher_events.array ) ;

    puts("### Waiting for the kernels to finish ......") ;
    waitForEvents( particle_pusher_events );
    releaseEventArray( particle_pusher_events ) ;

    finish_queue ( &gpu );

    free_kernel ( hello_world_kernel ) ;
    free_kernel ( particle_pusher.kernel ) ;
    free_dev_memory ( & data ) ;
    free_platform_struct ( &gpu ) ;


    return EXIT_SUCCESS;
}

void get_input_tex ( input_tex_tag_struct * input_tag ,  char * inputFile ){
    printf("### Reading %s ......\n" , inputFile ) ;
    init_read_input_tex( inputFile ) ;
    input_tag->grid_file = read_input_tex( "grid_file" ) ;
    input_tag->particle_position_file = read_input_tex( "particle_position_file" ) ;
    input_tag->particle_velocity_file = read_input_tex( "particle_velocity_file" ) ;
    close_read_input_tex() ;
}
#define p   data->particle
#define pCL data->particle_cl_mem
#define g   data->grid
#define gCL data->grid_cl_mem
void get_data_profile ( data_struct * data , input_tex_tag_struct * tag ) {
    
    // grid
    g.number = read_2D( & ( g.X ) , tag->grid_file ) ; 
    
    // particle
    p.number = read_2D( & ( p.X ) , tag->particle_position_file ) ;
    p.number = read_2D( & ( p.V ) , tag->particle_velocity_file ) ;
    
    //print_2D_list( grid->position      , grid->number      ) ;
    //print_2D_list( particles->position , particles->number ) ;
    //print_2D_list( particles->velocity , particles->number ) ;
}
void create_dev_memory ( data_struct * data , platform_struct * gpu ) {

    cl_int ret = 0 ;  
    
    // grid
    gCL.X_bytes = g.number * sizeof( grid_dimension ) ;
    gCL.X = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , gCL.X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

    // particle
    pCL.X_bytes = p.number * sizeof( particle_dimension ) ;
    pCL.V_bytes = p.number * sizeof( particle_dimension ) ;
    pCL.F_bytes = p.number * sizeof( particle_dimension ) ;

    pCL.X = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , pCL.X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    pCL.V = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , pCL.V_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
    pCL.F = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , pCL.F_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;
}
void free_dev_memory ( data_struct * data ) {
    
    // grid
    CL_CHECK( clReleaseMemObject( gCL.X ) ) ;
    
    // particle
    CL_CHECK( clReleaseMemObject( pCL.X ) ) ;
    CL_CHECK( clReleaseMemObject( pCL.V ) ) ;
    CL_CHECK( clReleaseMemObject( pCL.F ) ) ;
}
void write_data_to_device ( data_struct * data , cl_command_queue queue , cl_event * write_event ) {
    
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
    
    // grid
    CL_CHECK ( clEnqueueWriteBuffer( queue, gCL.X,  asynchronous, offset,  gCL.X_bytes, g.X, 0, NULL, write_event   ) ) ;
    
    // particle
    CL_CHECK ( clEnqueueWriteBuffer( queue, pCL.X,  asynchronous, offset,  pCL.X_bytes, p.X, 0, NULL, write_event+1 ) ) ;
    CL_CHECK ( clEnqueueWriteBuffer( queue, pCL.V,  asynchronous, offset,  pCL.V_bytes, p.V, 0, NULL, write_event+2 ) ) ;
}
#undef p
#undef g
#undef pCL
#undef gCL