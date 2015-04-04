//
//  world.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__world__
#define __FinalProject__world__

#include <stdio.h>
#include "entity.h"
#include "track.h"
#include "coaster_car.h"
#include "ground.h"
#include "skybox.h"

class World {
private:
    CoasterCar *car, *tail;
    Track *track;
    Ground *ground;
    Skybox *sky;
    Renderer *renderer;
    
    int cameraSpot;
    glm::vec3 camPos, camLook;
    
public:
    World(const char *filename);
    
    Track *getTrack() { return track; }
    
    void switchCamera();
    
    void update();
    void render();
};

#endif /* defined(__FinalProject__world__) */
