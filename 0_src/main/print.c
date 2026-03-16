#include <main_struct.h>
#include <stdio.h>

static char * annotation = "x , y" ;

void print_2D ( const unsigned int i ,
                const unsigned int N ,
                particle_dimension * P  ){
  
  char fName [128] ;  

  puts("printA") ;

  sprintf ( fName , "X_%i.tex", i ) ; 

  FILE * f_ptr = fopen( fName , "w") ;
  
  fprintf ( f_ptr , "#%s\n", annotation ) ;
  
  for ( unsigned int i = 0 ; i < N ; i++ ) {
    fprintf ( f_ptr , "% 1.5e , % 1.5e \n", P[i].x , P[i].y ) ;
  }

  fclose ( f_ptr ) ;

}

