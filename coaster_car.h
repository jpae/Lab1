//
//  coaster_car.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__coaster_car__
#define __FinalProject__coaster_car__

#include "entity.h"
#include "track.h"

class CoasterCar : public Entity {
private:
    CoasterCar *nextCar;
    Track *track, *start;
    float dist;
    
    float energy, kinetic;
    
    bool isFirstCar;
    bool cameraAttached;

public:
    CoasterCar();
    
    void setStart(Track *_track);
    void addCar(CoasterCar *car);
    void setCamera(bool attached);
    
    void update();
    void render();
};

#endif /* defined(__FinalProject__coaster_car__) */
