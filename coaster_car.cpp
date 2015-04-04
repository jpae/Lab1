//
//  coaster_car.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <glm/ext.hpp>
#include "coaster_car.h"
#include "bend.h"
#include "main.h"
#include "camera.h"

#define COASTER_SPEED 0.02f
#define COASTER_MASS 0.01f
#define COASTER_WIDTH 0.45f
#define FRICTION 0.0001f

#define LOTSA_SHAPES 10

bool camInFirstCar = false;

std::vector<tinyobj::shape_t> car_shapes;
std::vector<tinyobj::material_t> car_materials;
std::vector<float> car_posBuf[LOTSA_SHAPES], car_norBuf[LOTSA_SHAPES];
std::vector<unsigned int> car_indBuf[LOTSA_SHAPES];
bool car_initialized = false;

CoasterCar::CoasterCar() : nextCar(NULL), track(NULL), start(NULL), cameraAttached(false), isFirstCar(true), energy(COASTER_SPEED) {
    if (!car_initialized) {
        std::string err = tinyobj::LoadObj(car_shapes, car_materials, "models/car.obj");
        if(!err.empty()) {
            std::cerr << err << std::endl;
        }
        resize_obj(car_shapes);
        
        for(int s = 0; s < car_shapes.size(); s ++) {
            car_posBuf[s] = car_shapes[s].mesh.positions;
            car_indBuf[s] = car_shapes[s].mesh.indices;
            
            car_norBuf[s].clear();
            int idx1, idx2, idx3;
            glm::vec3 v1, v2, v3;
            //for every vertex initialize a normal to 0
            for (int j = 0; j < car_shapes[s].mesh.positions.size()/3; j++) {
                car_norBuf[s].push_back(0);
                car_norBuf[s].push_back(0);
                car_norBuf[s].push_back(0);
            }
            // DO work here to compute the normals for every face
            //then add its normal to its associated vertex
            for (int i = 0; i < car_shapes[s].mesh.indices.size()/3; i++) {
                idx1 = car_shapes[s].mesh.indices[3*i+0];
                idx2 = car_shapes[s].mesh.indices[3*i+1];
                idx3 = car_shapes[s].mesh.indices[3*i+2];
                v1 = glm::vec3(car_shapes[s].mesh.positions[3*idx1 +0],car_shapes[s].mesh.positions[3*idx1 +1], car_shapes[s].mesh.positions[3*idx1 +2]);
                v2 = glm::vec3(car_shapes[s].mesh.positions[3*idx2 +0],car_shapes[s].mesh.positions[3*idx2 +1], car_shapes[s].mesh.positions[3*idx2 +2]);
                v3 = glm::vec3(car_shapes[s].mesh.positions[3*idx3 +0],car_shapes[s].mesh.positions[3*idx3 +1], car_shapes[s].mesh.positions[3*idx3 +2]);
                
                glm::vec3 u, v;
                
                u = v2 - v1;
                v = v3 - v1;
                car_norBuf[s][3*idx1+0] += u.y * v.z - u.z * v.y;
                car_norBuf[s][3*idx1+1] += u.z * v.x - u.x * v.z;
                car_norBuf[s][3*idx1+2] += u.x * v.y - u.y * v.x;
                
                u = v3 - v2;
                v = v1 - v2;
                car_norBuf[s][3*idx2+0] += u.y * v.z - u.z * v.y;
                car_norBuf[s][3*idx2+1] += u.z * v.x - u.x * v.z;
                car_norBuf[s][3*idx2+2] += u.x * v.y - u.y * v.x;
                
                u = v1 - v3;
                v = v2 - v3;
                car_norBuf[s][3*idx3+0] += u.y * v.z - u.z * v.y;
                car_norBuf[s][3*idx3+1] += u.z * v.x - u.x * v.z;
                car_norBuf[s][3*idx3+2] += u.x * v.y - u.y * v.x;
            }
        }
        
        car_initialized = true;
    }
    
    for(int s = 0; s < car_shapes.size(); s ++) {
        Renderer *renderer = Program3D->create();
        
        renderer->setNumElements(car_indBuf[s].size());
        renderer->bufferData(INDICES_BUFFER, car_indBuf[s].size(), (void *)&car_indBuf[s][0]);
        
        renderer->bufferData(VERTEX_BUFFER, car_posBuf[s].size(), (void *)&car_posBuf[s][0]);
        renderer->bufferData(NORMAL_BUFFER, car_norBuf[s].size(), (void *)&car_norBuf[s][0]);
        
        renderers.push_back(renderer);
    }
    
    renderers[0]->mat = MATERIAL_RUBBER;
    renderers[1]->mat = MATERIAL_METAL;
    renderers[2]->mat = MATERIAL_RED_METAL;
    renderers[3]->mat = MATERIAL_CHAIR;
}

void CoasterCar::addCar(CoasterCar *car) {
    if (nextCar)
        nextCar->addCar(car);
    else {
        nextCar = car;
        
        nextCar->isFirstCar = false;
        
        nextCar->setStart(track);
        nextCar->dist = dist;
        nextCar->transformBefore(Model);
    }
    
    dist += COASTER_WIDTH;
}

void CoasterCar::setStart(Track *_track) {
    start = track = _track;
    dist = 0;
}

void CoasterCar::update() {
    if (isFirstCar) {
        glm::mat4 bend = glm::mat4(1.0f);
        if(track) {
            bend *= track->getModel();
            bend *= compute_bend(track->getBend(), dist);
        }
        
        // Compute kinetic energy from middle of car
        energy -= FRICTION;
        
        glm::vec3 pos(bend * Model * glm::vec4(0, 0, 0, 1));
        float potential = pos.y * COASTER_MASS;
        kinetic = energy - potential;
        
        // Car gets pulled along at a minimum velocity
        if (kinetic < COASTER_SPEED) {
            energy += COASTER_SPEED - kinetic;
            kinetic = COASTER_SPEED;
        }
    }
    
    dist += kinetic;
    
    while (track != NULL && dist >= 1.0f) {
        track = track->getNext();
        if (track == NULL) {
            track = start;
            energy = 0;
        }
        
        dist -= 1.0f;
    }
    
    if (nextCar) {
        nextCar->kinetic = kinetic;
        nextCar->update();
    }
}

void CoasterCar::setCamera(bool attached) {
    cameraAttached = attached;
    
    if (attached) {
        camInFirstCar = isFirstCar;
        
        if (isFirstCar)
            camera_setPosition(glm::vec3(0.1f, 0.35f, 0.4f));
        else
            camera_setPosition(glm::vec3(0.1f, 0.6f, 0.4f));
        camera_lookAt(glm::vec3(0.1f, 0.35f, -0.6f));
    }
}

void CoasterCar::render() {
    glm::mat4 bend = glm::mat4(1.0f);
    if (track) {
        bend *= track->getModel();
        bend *= compute_bend(track->getBend(), dist);
    }
    
    glm::mat4 M = Model;
    if (camInFirstCar)
        M *= glm::translate(0.0f, 0.0f, -1.0f);
    
    if (cameraAttached) {
        camera_setTransform(bend * M);
    }
    
    std::vector<Renderer *>::iterator renderer;
    for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
        (*renderer)->render(bend * M);
    
    Renderer::pushMatrix(bend * M);
    
    std::vector<Entity *>::iterator child;
    for (child = children.begin(); child != children.end(); child ++)
        (*child)->render();
    
    Renderer::popMatrix();
    
    if (nextCar)
        nextCar->render();
}