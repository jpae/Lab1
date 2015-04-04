//
//  bend.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include "main.h"
#include "bend.h"

glm::mat4 compute_bend(glm::vec3 bend, float theta) {
    glm::mat4 result = glm::mat4(1.0f);
    if (bend.y) {
        float inv = 1.0f / bend.y;
        result *= glm::translate(-inv, 0.0f, 0.0f);
        result *= glm::rotate(bend.y * theta * RADIANS_TO_DEG, 0.0f, -1.0f, 0.0f);
        result *= glm::translate(inv, 0.0f, 0.0f);
    }
    
    if (bend.x) {
        float inv = 1.0f / bend.x;
        
        result *= glm::translate(0.0f, inv, 0.0f);
        result *= glm::rotate(bend.x * theta * RADIANS_TO_DEG, -1.0f, 0.0f, 0.0f);
        result *= glm::translate(0.0f, -inv, 0.0f);
    }
    
    result *= glm::rotate(bend.z * theta * RADIANS_TO_DEG, 0.0f, 0.0f, -1.0f);
    
    if (bend.x == 0 && bend.y == 0)
        result *= glm::translate(0.0f, 0.0f, theta);
    
    return result;
}