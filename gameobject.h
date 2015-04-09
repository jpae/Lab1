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

class GameObject {
private:
   float x, y, z;

   std::vector<GameObject *> children;

   GraphicsComponent  *graphics;
   PhysicsComponent   *physics;
   Component          *input;
   CollisionComponent *collision;

public:
   Bounds bounds;

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

   void setX(float _x) { x = _x; }
   void setY(float _y) { y = _y; }
   void setZ(float _z) { z = _z; }

   void collide(GameObject *other);
   void update(World *world, float dt);
   void render();
   void _debug_render();
};

#endif
