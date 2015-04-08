//
//  gameobject.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//
#include <iostream>
#include <glm/ext.hpp>

#include "world.h"
#include "gameobject.h"

GameObject::GameObject(GraphicsComponent *g) 
   : x(0), y(0), z(0), graphics(g), physics(NULL), input(NULL) {
   children.clear();
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p)
   : x(0), y(0), z(0), graphics(g), physics(p), input(NULL) {
   children.clear();
}
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, Component *i) 
   : x(0), y(0), z(0), graphics(g), physics(p), input(i) {
   children.clear();
}

glm::mat4 GameObject::getModel() {
   return glm::translate(x, y, z);
}

void GameObject::update(World *world, float dt) {
   if (input)
      input->update(this);

   if (physics)
      physics->update(this, world, dt);
}

void GameObject::render() {
   if (graphics)
      graphics->render(this);
}
