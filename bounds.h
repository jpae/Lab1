//
//  bounds.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//

#ifndef __Lab1__Bounds__
#define __Lab1__Bounds__

typedef struct Bounds {
   float min_x, max_x;
   float min_y, max_y;
   float min_z, max_z;
} Bounds;

void _debug_drawBounds(Bounds& bounds);
void _debug_drawSphere(float radius);
void _debug_drawAxis();

#endif
