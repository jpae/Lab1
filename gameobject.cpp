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
   : x(0), y(0), z(0), speed(0), graphics(g), physics(NULL), input(NULL), 
   collision(NULL), type(OBJECT_OBSTACLE), collidesWith(0), remove(false) {
   children.clear();

   g->setBounds(this);
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p)
   : x(0), y(0), z(0), speed(0), graphics(g), physics(p), input(NULL), 
   collision(NULL), type(OBJECT_OBSTACLE), collidesWith(0), remove(false) {
   children.clear();

   g->setBounds(this);
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, Component *i) 
   : x(0), y(0), z(0), speed(0), graphics(g), physics(p), input(i), 
   collision(NULL), type(OBJECT_OBSTACLE), collidesWith(0), remove(false) {
   children.clear();

   g->setBounds(this);
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, Component *i,
   CollisionComponent *c) : x(0), y(0), z(0), speed(0), graphics(g), physics(p), 
   input(i), type(OBJECT_OBSTACLE), collidesWith(0), collision(c), remove(false) {
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
   glm::mat4 MV = glm::mat4(1.0f);
   float angle = glm::orientedAngle(glm::vec3(0, 0, 1.0f), this->getDirection(), glm::vec3(0, 1.0f, 0));
   MV *= glm::translate(this->getX(), this->getY(), this->getZ());
   MV *= glm::rotate(angle, glm::vec3(0, 1, 0));
   return MV;
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
      if (x + bounds.max_x >= other->x + other->bounds.min_x &&
          x + bounds.min_x <= other->x + other->bounds.max_x &&
          y + bounds.max_y >= other->y + other->bounds.min_y &&
          y + bounds.min_y <= other->y + other->bounds.max_y &&
          z + bounds.max_z >= other->z + other->bounds.min_z &&
          z + bounds.min_z <= other->z + other->bounds.max_z) {

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
   if (collision) {
      glPushMatrix();

      glm::mat4 Model = getModel();
      glMultMatrixf(&Model[0][0]);

      _debug_drawBounds(this->bounds);
      _debug_drawSphere(this->getRadius());
      _debug_drawAxis();

      glPopMatrix();
   }
}
