//
//  track.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/12/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__track__
#define __FinalProject__track__

#include "entity.h"

class Track : public Entity {
private:
    Track *nextTrack;
    bool changed;
    glm::mat4 ModelBend;
    
public:
    Track();
    
    void bend(glm::vec3 bend);
    glm::vec3 getBend() { return renderers[0]->bend; }
    
    void setNext(Track *child);
    Track *getNext() { return nextTrack; }
    
    void render();
};

#endif /* defined(__FinalProject__track__) */
