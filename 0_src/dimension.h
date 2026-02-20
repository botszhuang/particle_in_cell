#pragma once

typedef struct{
    double x ;
    double y ;
} dimension_2D_struct ;

#define copy_2D(dest,src) { dest.x = src.x ; dest.y = src.y ;}