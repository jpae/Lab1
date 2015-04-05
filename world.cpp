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

    entities.clear();

    Entity *bunny = new Entity();
    bunny->load("models/bunny.obj");

    entities.push_back(bunny);

    bunny = new Entity();
    bunny->load("models/bunny.obj");
    bunny->transformBefore(glm::translate(0.0f, 0.0f, 6.0f));

    entities.push_back(bunny);
}

void World::update() {
    camera_update();
}

void World::render() {
    std::vector<Entity *>::iterator iterator;
    for(iterator = entities.begin(); iterator < entities.end(); iterator ++) {
        (*iterator)->render();
    }
}
