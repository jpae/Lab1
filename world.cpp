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
    camera_setPosition(glm::vec3(0, 2, 0));
    camera_lookAt(glm::vec3(0, 0, 0));
}

void World::render() {
}

void World::update() {
}
