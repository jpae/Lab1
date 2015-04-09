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
#include "main.h"

const float time_per_spawn = 1.0f;
float t = 0;
World::World() {
    // Move camera
    camera_init();
    camera_setPosition(glm::vec3(0, 0, 3));
    camera_lookAt(glm::vec3(0, 0, 0));

    objects.clear();

    // Bunny 1
    GameObject *bunny = new GameObject(new ModelRenderer("models/bunny.obj"), 
        new MovementComponent(), new PlayerInputComponent(), new PlayerCollisionComponent());
    // bunny->setSpeed(3.0);
    bunny->type = OBJECT_PLAYER;

    objects.push_back(bunny);

    // Bunny 2
    bunny = new GameObject(new ModelRenderer("models/bunny.obj"), NULL, NULL,
        new CollisionComponent());
    bunny->type = OBJECT_TARGET;
    bunny->setZ(6);
    objects.push_back(bunny);

    GameObject *ground = new GameObject(new GroundRenderer(10));
    objects.push_back(ground);
}

void World::collide(GameObject *obj) {
    std::vector<GameObject *>::iterator iterator;
    for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
        if (*iterator != obj && (*iterator)->collidesWith & obj->type)
            (*iterator)->collide(obj);
    }
}

void World::update(float dt) {
    camera_update();

    t += dt;
    if (t >= time_per_spawn) {
        // Create a new object
        GameObject *newObject = new GameObject(new ModelRenderer("models/bunny.obj"), NULL, NULL,
            new CollisionComponent());
        newObject->type = OBJECT_TARGET;
        std::cout << "New obj!" << std::endl;

        t -= time_per_spawn;
    }

    std::vector<GameObject *>::iterator iterator;
    for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
        (*iterator)->update(this, dt);
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
}
