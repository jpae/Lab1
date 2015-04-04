//
//  bend.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__bend__
#define __FinalProject__bend__

#include <stdio.h>
#include <glm/glm.hpp>

/* 
 * Compute the bend of a model on x, y, and z
 * theta is a value in [0, 1], with 0 being
 * a non-progressed bend and 1 being the bend
 * after completion
 */
glm::mat4 compute_bend(glm::vec3 bend, float theta);

#endif /* defined(__FinalProject__bend__) */
