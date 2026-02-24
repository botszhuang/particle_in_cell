#include <stdlib.h>
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
    cl_mem mass ;
    cl_mem dt ;
    size_t X_bytes ;
    size_t V_bytes ;
    size_t F_bytes ;
    size_t mass_bytes ;
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
   myfloat current_T ;
   myfloat dt ;
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
void read_data_to_host ( data_struct * data , cl_command_queue queue , cl_event_struct wait , cl_event_struct read ) ;

int main( int argc , char * argv[]  ){

    if ( argc < 2 ){
        printf("Plaese provide the input file as the first argument.\n") ;
        return EXIT_FAILURE ;
    }

    char * inputFile = argv[1] ;
    printLine() ; 
    printf("Hello world!\n") ;

    // --------------------------------------------------------------------------
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
    // --------------------------------------------------------------------------

    // read input.tex
    input_tex_tag_struct input_tag ;
    get_input_tex ( &input_tag , inputFile ) ;

    data_struct data;
 
    puts("### Reading grid and particle profiles ......") ;
    get_data_profile ( &data , &input_tag ) ;

    // --------------------------------------------------------------------------
    // events
    const cl_int write_grid_count     = 1 ;
    const cl_int write_particle_count = 2 ;
    cl_event_struct write_events = initEventStruct ( write_grid_count + write_particle_count ) ;
    
    cl_event_struct leap_frog_V_half_events = initEventStruct( 1 ) ;
    cl_event_struct leap_frog_X_events = initEventStruct( 1 ) ;
    cl_event_struct force_events = initEventStruct ( 1 ) ;

    const cl_int read_event_count = 3 ;
    cl_event_struct read_events = initEventStruct ( read_event_count ) ;

    cl_uint work_dim = 1 ;
    size_t global_work_offset[1] = {0} ;
    size_t global_work_size[1] = {1} ;
    size_t local_work_size[1] = {1} ;

    // kernels
    leap_frog_kernel_struct leap_frog ;
    force_kernel_struct force ;

    // --------------------------------------------------------------------------

    puts("### Creating kernels ......") ;
    create_leap_frog_kernel ( &leap_frog, gpu,  work_dim, global_work_offset, global_work_size, local_work_size ) ; 
    create_force_G_kernel ( &force, gpu,  work_dim, global_work_offset, global_work_size, local_work_size ) ; 

    puts("### Allocating memory for grid and particle on the device ......") ;
    create_dev_memory ( &data , &gpu ) ;

    puts("### Setting the arguments for kernels ......") ;
    set_leap_frog_kernel_args ( & leap_frog , 
                                &( data.particle_cl_mem.X ), &( data.particle_cl_mem.V ), &( data.particle_cl_mem.F ) ,
                                &( data.particle.number   ), &( data.dt )  ) ;
    set_force_G_kernel_args ( & force , &( data.particle_cl_mem.F ), &( data.particle.number ) ) ;

    // --------------------------------------------------------------------------
    puts("### Writing grid and particle data to the device ......") ;
    write_data_to_device ( &data  , gpu.queue[0] , write_events.array ) ;
    //waitForEvents ( write_events ) ;

    puts("### Running kernels ......") ;
    
    // leap frog step 0 : init V_half
    run_init_leap_frog_V_half_kernel ( & leap_frog , data.dt , gpu.queue[0] , write_events , leap_frog_V_half_events.array ) ;
    releaseEventArray( write_events ) ;

    //for ( unsigned int i = 0 ; i < read_event_count ; i++ ) { clSetUserEventStatus ( read_events.array [ i ] , CL_COMPLETE ) ; }

    //for ( unsigned int i = 0 ; i < 5 ; i++ ) {
        // leap frog step 1 :
        run_leap_frog_X_kernel ( & leap_frog , gpu.queue[0] , leap_frog_V_half_events , leap_frog_X_events.array ) ;
        releaseEventArray( leap_frog_V_half_events ) ;
        
        // leap frog step 2 :   a = A(x)  compute A
        run_force_G_kernel ( & force , gpu.queue[0] , leap_frog_X_events , force_events.array ) ;

        // leap frog step 3 :
        run_leap_frog_V_half_kernel ( & leap_frog , gpu.queue[0] , force_events , leap_frog_V_half_events.array ) ;
 
        releaseEventArray( leap_frog_X_events ) ;       
        releaseEventArray( force_events ) ;

        //if ( ( i+1 )% 5 ) { continue; }
        //waitForEvents ( read_events ) ;
        //releaseEventArray ( read_events ) ;
        read_data_to_host ( & data , gpu.queue[0] , leap_frog_V_half_events , read_events ) ;

    //}
    puts("### Waiting for the kernels to finish ......") ;
    waitForEvents( leap_frog_V_half_events );
    releaseEventArray( leap_frog_V_half_events ) ;
    waitForEvents ( read_events ) ;
    releaseEventArray ( read_events ) ;


    // --------------------------------------------------------------------------
    finish_queue ( &gpu );
    
    free_kernel ( hello_world_kernel ) ;
    free_leap_frog_kernel ( & leap_frog ) ;
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
    
    data->current_T = 0 ;
    data->dt = 0.1 ;

    // grid
    g.number = read_2D( & ( g.X ) , tag->grid_file ) ; 
    
    // particle
    p.number = read_2D( & ( p.X ) , tag->particle_position_file ) ;
    p.number = read_2D( & ( p.V ) , tag->particle_velocity_file ) ;

    p.F = calloc (  p.number , sizeof ( p.F[0] ) ) ;
    
    //print_2D_list( grid->position      , grid->number      ) ;
    //print_2D_list( particles->position , particles->number ) ;
    //print_2D_list( particles->velocity , particles->number ) ;

    if ( p.X == NULL || p.V == NULL || p.F == NULL) {
        fprintf(stderr, "Host memory allocation failed!\n");
        exit (1) ;
    }
}
void create_dev_memory ( data_struct * data , platform_struct * gpu ) {

    cl_int ret = 0 ;  
    
    // grid
    gCL.X_bytes = g.number * sizeof( grid_dimension ) ;
    gCL.X = clCreateBuffer( gpu->context , CL_MEM_READ_WRITE , gCL.X_bytes , NULL , &ret ) ; CL_CHECK( ret ) ;

    // particle
    pCL.X_bytes = p.number * sizeof( p.X[0] ) ;
    pCL.V_bytes = p.number * sizeof( p.V[0] ) ;
    pCL.F_bytes = p.number * sizeof( p.F[0] ) ;
    
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
void read_data_to_host ( data_struct * data , cl_command_queue queue , cl_event_struct wait , cl_event_struct read ) {
    
    const cl_bool asynchronous = CL_FALSE ;
    const unsigned int offset = 0 ;
      
    // particle
    CL_CHECK ( clEnqueueReadBuffer( queue, pCL.X,  asynchronous, offset,  pCL.X_bytes, p.X, wait.count, wait.array , read.array   ) ) ;
    CL_CHECK ( clEnqueueReadBuffer( queue, pCL.V,  asynchronous, offset,  pCL.V_bytes, p.V, wait.count, wait.array , ( read.array )+1 ) ) ;
    CL_CHECK ( clEnqueueReadBuffer( queue, pCL.F,  asynchronous, offset,  pCL.F_bytes, p.F, wait.count, wait.array , ( read.array )+2 ) ) ;
}
#undef p
#undef g
#undef pCL
#undef gCL