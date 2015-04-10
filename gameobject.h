//
//  gameobject.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//

#ifndef __Project__gameobject__
#define __Project__gameobject__

#include <glm/glm.hpp>
#include <vector>

#include "component.h"

#define OBJECT_PLAYER 0
#define OBJECT_TARGET 1
#define OBJECT_OBSTACLE 2

class GameObject {
private:
   float x, y, z;
   float speed;
   glm::vec3 direction;

   std::vector<GameObject *> children;
   

   GraphicsComponent  *graphics;
   PhysicsComponent   *physics;
   Component          *input;
   CollisionComponent *collision;

public:
   Bounds bounds;
   unsigned int type;
   unsigned int collidesWith;

   bool remove;

   GameObject(GraphicsComponent  *graphics, 
              PhysicsComponent   *physics,
              Component          *input,
              CollisionComponent *collision);
   GameObject(GraphicsComponent  *graphics, 
              PhysicsComponent   *physics,
              Component          *input);
   GameObject(GraphicsComponent  *graphics, 
              PhysicsComponent   *physics);
   GameObject(GraphicsComponent  *graphics);

   glm::mat4 getModel();

   float getX() { return x; }
   float getY() { return y; }
   float getZ() { return z; }
   float getRadius();
   float getSpeed() { return speed; }
   glm::vec3 getDirection() { return direction; } 

   void setX(float _x) { x = _x; }
   void setY(float _y) { y = _y; }
   void setZ(float _z) { z = _z; }
   void setSpeed(float _s) { speed = _s; }
   void setDirection(glm::vec3 _d) { direction = glm::normalize(_d); }

   void collide(GameObject *other);
   void update(World *world, float dt);
   void render();
   void _debug_render();
};

#endif
