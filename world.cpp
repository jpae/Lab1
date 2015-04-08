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

World::World() {
    // Move camera
    camera_init();
    camera_setPosition(glm::vec3(0, 0, 3));
    camera_lookAt(glm::vec3(0, 0, 0));

    objects.clear();

    GameObject *bunny = new GameObject(new ModelRenderer("models/bunny.obj"), NULL, new PlayerInputComponent());

    objects.push_back(bunny);

    bunny = new GameObject(new ModelRenderer("models/bunny.obj"));
    bunny->setZ(6);

    objects.push_back(bunny);
}

void World::update(float dt) {
    camera_update();

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
}
