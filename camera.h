//
//  camera.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/11/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__camera__
#define __FinalProject__camera__

#include <stdio.h>
#include <glm/glm.hpp>

void camera_setPosition(glm::vec3 position);
void camera_setTransform(glm::mat4 _transform);
void camera_lookAt(glm::vec3 dest);
void camera_setPitch(double pitch);
void camera_movePitch(double dp);
void camera_setYaw(double yaw);
void camera_moveYaw(double dy);
void camera_move(float dx, float dy, float dz);

void camera_init();
void camera_update();

glm::mat4 camera_getMatrix();
glm::vec3 camera_getPosition();
glm::vec3 camera_getLookAt();

#endif /* defined(__FinalProject__camera__) */
