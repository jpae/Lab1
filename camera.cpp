//
//  camera.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/11/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include "main.h"
#include "camera.h"

glm::vec3 position;
glm::mat4 transform(1.0f);
bool inCar = false;
double pitch, yaw;

void camera_init() {
}

void camera_setPosition(glm::vec3 _position) {
    position = _position;
}

void camera_setTransform(glm::mat4 _transform) {
    transform = _transform;
    inCar = (transform != glm::mat4(1.0f));
}

void camera_update() { }

glm::vec3 camera_getPosition() { return position; }
glm::vec3 camera_getLookAt() {
    float y = yaw;
    float p = pitch;
    
    return glm::vec3(cos(y) * cos(p), sin(p), -sin(y) * cos(p));
}

void camera_lookAt(glm::vec3 dest) {
    glm::vec3 dir = glm::normalize(dest - position);
    
    ;
    camera_setPitch(asin(dir.y));
    yaw = acos(dir.x / cos(pitch));
    if (dir.z > 0) yaw *= -1;

    // camera_setYaw(yaw);
}

void camera_setPitch(double _pitch) {
    pitch = _pitch;
    if (pitch > MATH_PI * 5 / 11)
        pitch = MATH_PI * 5 / 11;
    if (pitch < -MATH_PI * 5 / 11)
        pitch = -MATH_PI * 5 / 11;
}

void camera_movePitch(double dp) {
    camera_setPitch(pitch + dp);
}

void camera_setYaw(double _yaw) {
    yaw = _yaw;
}

void camera_moveYaw(double dy) {
    yaw += dy;
}

void camera_move(float dx, float dy, float dz) {
    position += dz * glm::vec3(cos(yaw), 0, -sin(yaw));
    position += dy * glm::vec3(0, 1, 0);
    position += dx * glm::cross(glm::vec3(cos(yaw), 0, -sin(yaw)), glm::vec3(0, 1, 0));
}

glm::mat4 camera_getMatrix() {
    glm::vec4 pos = transform * glm::vec4(position, 1);
    glm::vec4 dir = transform * glm::vec4(camera_getLookAt(), 0);
    glm::vec4 up = transform * glm::vec4(0, 1, 0, 0);
    
    return glm::translate(glm::vec3(0, 0, -3)) * glm::lookAt(glm::vec3(pos), glm::vec3(pos + dir), glm::vec3(up));
}
