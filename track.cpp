//
//  track.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/12/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <string>
#include <glm/ext.hpp>
#include "track.h"
#include "main.h"
#include "bend.h"

std::vector<tinyobj::shape_t> track_shapes;
std::vector<tinyobj::material_t> track_materials;
std::vector<float> track_posBuf, track_norBuf;
std::vector<unsigned int> track_indBuf;
bool track_initialized = false;

Track::Track() : nextTrack(NULL), ModelBend(glm::translate(0, 0, 1)), changed(true) {
    if (!track_initialized) {
        std::string err = tinyobj::LoadObj(track_shapes, track_materials, "models/track.obj");
        if(!err.empty()) {
            std::cerr << err << std::endl;
        }
        resize_obj(track_shapes);
        
        for(int s = 0; s < track_shapes.size(); s ++) {
            track_posBuf = track_shapes[s].mesh.positions;
            track_indBuf = track_shapes[s].mesh.indices;
            
            track_norBuf.clear();
            int idx1, idx2, idx3;
            glm::vec3 v1, v2, v3;
            //for every vertex initialize a normal to 0
            for (int j = 0; j < track_shapes[s].mesh.positions.size()/3; j++) {
                track_norBuf.push_back(0);
                track_norBuf.push_back(0);
                track_norBuf.push_back(0);
            }
            // DO work here to compute the normals for every face
            //then add its normal to its associated vertex
            for (int i = 0; i < track_shapes[s].mesh.indices.size()/3; i++) {
                idx1 = track_shapes[s].mesh.indices[3*i+0];
                idx2 = track_shapes[s].mesh.indices[3*i+1];
                idx3 = track_shapes[s].mesh.indices[3*i+2];
                v1 = glm::vec3(track_shapes[s].mesh.positions[3*idx1 +0],track_shapes[s].mesh.positions[3*idx1 +1], track_shapes[s].mesh.positions[3*idx1 +2]);
                v2 = glm::vec3(track_shapes[s].mesh.positions[3*idx2 +0],track_shapes[s].mesh.positions[3*idx2 +1], track_shapes[s].mesh.positions[3*idx2 +2]);
                v3 = glm::vec3(track_shapes[s].mesh.positions[3*idx3 +0],track_shapes[s].mesh.positions[3*idx3 +1], track_shapes[s].mesh.positions[3*idx3 +2]);
                
                glm::vec3 u, v;
                
                u = v2 - v1;
                v = v3 - v1;
                track_norBuf[3*idx1+0] += u.y * v.z - u.z * v.y;
                track_norBuf[3*idx1+1] += u.z * v.x - u.x * v.z;
                track_norBuf[3*idx1+2] += u.x * v.y - u.y * v.x;
                
                u = v3 - v2;
                v = v1 - v2;
                track_norBuf[3*idx2+0] += u.y * v.z - u.z * v.y;
                track_norBuf[3*idx2+1] += u.z * v.x - u.x * v.z;
                track_norBuf[3*idx2+2] += u.x * v.y - u.y * v.x;
                
                u = v1 - v3;
                v = v2 - v3;
                track_norBuf[3*idx3+0] += u.y * v.z - u.z * v.y;
                track_norBuf[3*idx3+1] += u.z * v.x - u.x * v.z;
                track_norBuf[3*idx3+2] += u.x * v.y - u.y * v.x;
            }
        }
        
        track_initialized = true;
    }
    
    Renderer *renderer = Program3D->create();
    
    renderer->setNumElements(track_indBuf.size());
    renderer->bufferData(INDICES_BUFFER, track_indBuf.size(), (void *)&track_indBuf[0]);
    
    renderer->bufferData(VERTEX_BUFFER, track_posBuf.size(), (void *)&track_posBuf[0]);
    renderer->bufferData(NORMAL_BUFFER, track_norBuf.size(), (void *)&track_norBuf[0]);
    
    renderers.push_back(renderer);
}

void Track::bend(glm::vec3 bend) {
    this->renderers[0]->bend = bend;
    
    ModelBend = compute_bend(bend, 1);
    
    changed = true;
}

void Track::setNext(Track *child) {
    this->nextTrack = child;
}

void Track::render() {
    Entity::render();
    
    // If model changed, recompute children
    if (this->nextTrack) {
        if (changed) {
            this->nextTrack->transformAfter(Model * ModelBend);
            this->nextTrack->changed = true;
            
            changed = false;
        }
    
        this->nextTrack->render();
    }
}
