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
#include "coaster_car.h"

World::World(const char *filename) : car(new CoasterCar()), track(new Track()), renderer(Program3D->create()), cameraSpot(0), ground(new Ground()), sky(new Skybox()) {
    // Move camera
    camera_init();
    camera_setPosition(glm::vec3(0, 2, 0));
    camera_lookAt(glm::vec3(0, 2, -1));
    
    // Create initial track piece
    track->transformBefore(glm::scale(2.0f, 2.0f, 2.0f));
    Track *track = this->track;
    Track *next;
    
    // Load track from file
    std::ifstream trackFile(filename);
    glm::vec3 bend;
    std::string line;
    while (std::getline(trackFile, line)) {
        std::istringstream iss(line);
        if (iss.str().find("//") != std::string::npos ||
            iss.str().length() == 0) {
            // Ignore ...
        }
        else if (iss >> bend.x >> bend.y >> bend.z) {
            // Track input is degrees, convert to radians
            bend *= DEG_TO_RADIANS;
            
            next = new Track();
            next->bend(bend);
            
            track->setNext(next);
            track = next;
        }
        else {
            std::cerr << "Improper track line: " << iss.str() << std::endl;
        }
    }
    
    while (trackFile >> bend.x >> bend.y >> bend.z) {
        // Track input is degrees, convert to radians
        bend *= DEG_TO_RADIANS;
        
        next = new Track();
        next->bend(bend);
        
        track->setNext(next);
        track = next;
    }
    
    car->setStart(this->track);
    
    float car_sc = 1.0f / 2.6f;
    car->transformBefore(glm::scale(car_sc, car_sc, car_sc));
    car->transformBefore(glm::translate(0.0f, 0.34f, -0.5f));
    
    for(int i = 0; i < NUM_CARS; i ++) {
        tail = new CoasterCar();
        car->addCar(tail);
    }
}

void World::switchCamera() {
    cameraSpot = (cameraSpot + 1) % 3;
    if (cameraSpot == 1) {
        camPos = camera_getPosition();
        camLook = camera_getLookAt();
        car->setCamera(true);
    }
    else if (cameraSpot == 2) {
        car->setCamera(false);
        tail->setCamera(true);
    }
    else {
        tail->setCamera(false);
        camera_setPosition(camPos);
        camera_lookAt(camPos + camLook * glm::vec3(1, 1, -1));
        camera_setTransform(glm::mat4(1.0f));
    }
}

void World::render() {
    ground->render();
    car->render();
    
    track->render();
    sky->render();
}

void World::update() {
    if (cameraSpot != 0)
        camera_update();
    
    car->update();
}