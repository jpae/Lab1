//
//  bounds.cpp
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//
#include <iostream>

#include "GLSL.h"
#include "main.h"
#include "bounds.h"

/* DEBUG: Draw a bounding box around an object */
void _debug_drawBounds(Bounds& bounds) {
   glBegin(GL_LINES);
      glColor3f(1, 0, 0);
      glVertex3f(bounds.min_x, bounds.min_y, bounds.min_z);
      glVertex3f(bounds.max_x, bounds.min_y, bounds.min_z);
      glVertex3f(bounds.min_x, bounds.min_y, bounds.min_z);
      glVertex3f(bounds.min_x, bounds.max_y, bounds.min_z);
      glVertex3f(bounds.min_x, bounds.min_y, bounds.min_z);
      glVertex3f(bounds.min_x, bounds.min_y, bounds.max_z);

      glVertex3f(bounds.min_x, bounds.max_y, bounds.max_z);
      glVertex3f(bounds.max_x, bounds.max_y, bounds.max_z);
      glVertex3f(bounds.min_x, bounds.max_y, bounds.max_z);
      glVertex3f(bounds.min_x, bounds.min_y, bounds.max_z);
      glVertex3f(bounds.min_x, bounds.max_y, bounds.max_z);
      glVertex3f(bounds.min_x, bounds.max_y, bounds.min_z);

      glVertex3f(bounds.max_x, bounds.max_y, bounds.min_z);
      glVertex3f(bounds.min_x, bounds.max_y, bounds.min_z);
      glVertex3f(bounds.max_x, bounds.max_y, bounds.min_z);
      glVertex3f(bounds.max_x, bounds.min_y, bounds.min_z);
      glVertex3f(bounds.max_x, bounds.max_y, bounds.min_z);
      glVertex3f(bounds.max_x, bounds.max_y, bounds.max_z);

      glVertex3f(bounds.max_x, bounds.min_y, bounds.max_z);
      glVertex3f(bounds.min_x, bounds.min_y, bounds.max_z);
      glVertex3f(bounds.max_x, bounds.min_y, bounds.max_z);
      glVertex3f(bounds.max_x, bounds.max_y, bounds.max_z);
      glVertex3f(bounds.max_x, bounds.min_y, bounds.max_z);
      glVertex3f(bounds.max_x, bounds.min_y, bounds.min_z);
   glEnd();
}

#define CIRCLE_GRANULARITY 15
void _debug_drawSphere(float radius) {
   glBegin(GL_LINES);
      glColor3f(1, 1, 0);
      for(int degrees = 0; degrees * CIRCLE_GRANULARITY < 360; degrees ++) {
         glVertex3f(0, radius * sinf(DEG_TO_RADIANS * degrees * CIRCLE_GRANULARITY),       radius * cosf(DEG_TO_RADIANS * degrees * CIRCLE_GRANULARITY));
         glVertex3f(0, radius * sinf(DEG_TO_RADIANS * (degrees + 1) * CIRCLE_GRANULARITY), radius * cosf(DEG_TO_RADIANS * (degrees + 1) * CIRCLE_GRANULARITY));
      
         glVertex3f(radius * sinf(DEG_TO_RADIANS * degrees * CIRCLE_GRANULARITY),       0, radius * cosf(DEG_TO_RADIANS * degrees * CIRCLE_GRANULARITY));
         glVertex3f(radius * sinf(DEG_TO_RADIANS * (degrees + 1) * CIRCLE_GRANULARITY), 0, radius * cosf(DEG_TO_RADIANS * (degrees + 1) * CIRCLE_GRANULARITY));
      
         glVertex3f(radius * sinf(DEG_TO_RADIANS * degrees * CIRCLE_GRANULARITY),       radius * cosf(DEG_TO_RADIANS * degrees * CIRCLE_GRANULARITY), 0);
         glVertex3f(radius * sinf(DEG_TO_RADIANS * (degrees + 1) * CIRCLE_GRANULARITY), radius * cosf(DEG_TO_RADIANS * (degrees + 1) * CIRCLE_GRANULARITY), 0);
      }
   glEnd();
}

void _debug_drawAxis() {
   glLineWidth(10.0f);
   glBegin(GL_LINES);
      glColor3f(1, 0, 0);
      glVertex3f(0, 0, 0);
      glVertex3f(2, 0, 0);
      glColor3f(0, 1, 0);
      glVertex3f(0, 0, 0);
      glVertex3f(0, 2, 0);
      glColor3f(0, 0, 1);
      glVertex3f(0, 0, 0);
      glVertex3f(0, 0, 2);
   glEnd();
   glLineWidth(1.0f);
}
