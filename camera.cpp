//
//  camera.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/11/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <noise/noise.h>
#include <glm/ext.hpp>
#include "main.h"
#include "camera.h"

glm::vec3 position;
glm::mat4 transform(1.0f);
bool inCar = false;
double pitch, yaw;

#define SHAKE_SCALE 0.06f
noise::module::Perlin shake;
float t = 0;

void camera_init() {
}

void camera_setPosition(glm::vec3 _position) {
    position = _position;
}

void camera_setTransform(glm::mat4 _transform) {
    transform = _transform;
    inCar = (transform != glm::mat4(1.0f));
}

void camera_update() { t += 0.01f; }

glm::vec3 camera_getPosition() { return position; }
glm::vec3 camera_getLookAt() {
    float y = yaw;
    float p = pitch;
    if (inCar) {
        y += shake.GetValue(0, t, 0) * SHAKE_SCALE;
        p += shake.GetValue(t, 0, 0) * SHAKE_SCALE;
    }
    
    return glm::vec3(cos(y) * cos(p), sin(p), -sin(y) * cos(p));
}

void camera_lookAt(glm::vec3 dest) {
    glm::vec3 dir = glm::normalize(dest - position);
    
    pitch = asin(dir.y);
    yaw = acos(dir.x / cos(pitch));
    if (dir.z < 0) yaw *= -1;
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
    
    return glm::lookAt(glm::vec3(pos), glm::vec3(pos + dir), glm::vec3(up));
}