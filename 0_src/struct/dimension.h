#ifndef DIMENSION_H
#define DIMENSION_H

#define myfloat double

typedef struct{
    myfloat x ;
    myfloat y ;
} dimension_2D_struct ;

#define particle_dimension dimension_2D_struct
#define grid_dimension     dimension_2D_struct

#define copy_2D(dest,src) { dest.x = src.x ; dest.y = src.y ;}
//#define add_2D(dest,src,m)       { dest.x = src.x + m.x ; dest.y = src.y + m.y ; }
//#define div_2D(dest,src,m)       { dest.x = src.x / m ; dest.y = src.y / m ; }
//#define multiple_2D(dest,src,m)  { dest.x = src.x * m ; dest.y = src.y * m ; }
#endif

