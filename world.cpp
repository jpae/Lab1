//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include "world.h"
#include "camera.h"
#include "renderer.h"
#include "main.h"

const float time_per_spawn = 1.0f;
float t = 0;


World::World() {
   // Move camera
   camera_init();
   camera_setPosition(glm::vec3(0, 2, 0));
   camera_lookAt(glm::vec3(10, 2, 0));

   objects.clear();

   player = new PlayerCollisionComponent();
   GameObject *p = new GameObject(new ModelRenderer("models/car.obj"), 
    new PlayerMovementComponent(), new PlayerInputComponent(), player);
   p->type = OBJECT_PLAYER;
   p->collidesWith = OBJECT_TARGET;
   p->setY(1);
   p->setDirection(glm::vec3(camera_getLookAt()));
   objects.push_back(p);

   GameObject *ground = new GameObject(new GroundRenderer(GROUND_WIDTH/2));
   objects.push_back(ground);
}

void World::addObject(GameObject *obj) {
   objects.push_back(obj);
}

void World::collide(GameObject *obj) {
   std::vector<GameObject *>::iterator iterator;
   for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
      if (*iterator != obj && obj->collidesWith & (*iterator)->type) {
         obj->collide(*iterator);
      }
   }
}

void World::update(float dt) {
   camera_update();

   t += dt;
   if (t >= time_per_spawn) {
      // Create a new object
      GameObject *newObject = new GameObject(new ModelRenderer("models/bunny.obj"), 
         new MovementComponent(), NULL, new TargetCollisionComponent());
      newObject->type = OBJECT_TARGET;
      newObject->collidesWith = OBJECT_TARGET;
      newObject->setY(1);
      newObject->setSpeed(randFloat(5.0f, 10.0f));
      newObject->setDirection(glm::vec3(randFloat(-1.0, 1.0), 0, randFloat(-1.0, 1.0)));

      addObject(newObject);

      t -= time_per_spawn;
   }
   
   std::vector<GameObject *>::iterator iterator = objects.begin();
   while(iterator < objects.end()) {
      (*iterator)->update(this, dt);
      this->collide(*iterator);
      if ((*iterator)->remove)
         iterator = objects.erase(iterator);
      else
         iterator ++;
   }
}

void World::render() {
   std::vector<GameObject *>::iterator iterator;
   for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
      (*iterator)->render();
   }

   #ifdef DEBUG
   glm::mat4 View = camera_getMatrix();

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(&View[0][0]);

   for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
      (*iterator)->_debug_render();
   }
   #endif

   char score[16];
   sprintf(score, "Score: %d", player->score);
   renderText(score, 50, 700);
}
