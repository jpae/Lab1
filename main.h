//
//  main.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef FinalProject_main_h
#define FinalProject_main_h

#include <vector>
#include <glm/glm.hpp>
#include "tiny_obj_loader.h"

#define MATH_PI 3.1415926535897932384626433832795
#define RADIANS_TO_DEG float(180.0f / MATH_PI)
#define DEG_TO_RADIANS float(MATH_PI / 180.0f)

#define NUM_CARS 3
#define TRACK "tracks/track1.trk"

const extern int w_width;
const extern int w_height;

void resize_obj(std::vector<tinyobj::shape_t> &shapes);

// Debugging operators
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec);
std::ostream &operator<< (std::ostream &out, const glm::mat4 &vec);

#endif
