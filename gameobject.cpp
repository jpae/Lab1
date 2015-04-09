//
//  gameobject.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//
#include <iostream>
#include <glm/ext.hpp>

#include "main.h"
#include "world.h"
#include "gameobject.h"

/* Constructors (ew) */
GameObject::GameObject(GraphicsComponent *g) 
   : x(0), y(0), z(0), graphics(g), physics(NULL), input(NULL), collision(NULL) {
   children.clear();

   g->setBounds(this);
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p)
   : x(0), y(0), z(0), graphics(g), physics(p), input(NULL), collision(NULL) {
   children.clear();

   g->setBounds(this);
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, Component *i) 
   : x(0), y(0), z(0), graphics(g), physics(p), input(i), collision(NULL) {
   children.clear();

   g->setBounds(this);
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, Component *i,
   CollisionComponent *c) : x(0), y(0), z(0), graphics(g), physics(p), input(i), 
   collision(c) {
   children.clear();

   g->setBounds(this);
}

float GameObject::getRadius() {
   float max = -1 * bounds.min_x;

   if (max < bounds.max_x)
      max = bounds.max_x;
   if (max < bounds.max_y)
      max = bounds.max_y;
   if (max < bounds.max_z)
      max = bounds.max_z;

   if (max < -1 * bounds.min_y)
      max = -1 * bounds.min_y;
   if (max < -1 * bounds.min_z)
      max = -1 * bounds.min_z;

   assert(max >= 0);

   return max;
}

glm::mat4 GameObject::getModel() {
   return glm::translate(x, y, z);
}

void GameObject::collide(GameObject *other) {
   if (!collision || !other->collision)
      return;

   float max_dist = getRadius() + other->getRadius();
   float dx = x - other->x;
   float dy = y - other->y;
   float dz = z - other->z;

   // Within each other's sphere
   if (max_dist * max_dist >= dx * dx + dy * dy + dz * dz) {
      // Check bounding boxes
      if (bounds.max_x >= other->bounds.min_x &&
          bounds.min_x <= other->bounds.max_x &&
          bounds.max_y >= other->bounds.min_y &&
          bounds.min_y <= other->bounds.max_y &&
          bounds.max_z >= other->bounds.min_z &&
          bounds.min_z <= other->bounds.max_z) {

         // Collided
         collision->collide(this, other);
         other->collision->collide(other, this);
      }
   }
}

void GameObject::update(World *world, float dt) {
   if (input)
      input->update(this);

   if (physics)
      physics->update(this, world, dt);

   if (collision)
      world->collide(this);
}

void GameObject::render() {
   if (graphics)
      graphics->render(this);
}

void GameObject::_debug_render() {
   glPushMatrix();

   glm::mat4 Model = getModel();
   glMultMatrixf(&Model[0][0]);

   _debug_drawBounds(this->bounds);
   _debug_drawSphere(this->getRadius());

   glPopMatrix();
}
