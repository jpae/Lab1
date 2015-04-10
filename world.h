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
#include <vector>
#include "gameobject.h"

class World {
private:
    std::vector<GameObject *> objects;
    
public:
    World();
    
    void addObject(GameObject *obj);

    void collide(GameObject *obj);
    void update(float dt);
    void render();
};

#endif /* defined(__FinalProject__world__) */
