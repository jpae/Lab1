//
//  entity.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include "entity.h"
#include "main.h"

using namespace std;

Entity::Entity() : Model(glm::mat4(1)) {
    renderers.clear();
    children.clear();
};

void Entity::load(const char *filename) {
    string err = tinyobj::LoadObj(shapes, materials, filename);
    if(!err.empty()) {
        cerr << err << endl;
    }
    resize_obj(shapes);
    
    for(int s = 0; s < shapes.size(); s ++) {
        const vector<float> &posBuf = shapes[s].mesh.positions;
        
        vector<float> norBuf;
        int idx1, idx2, idx3;
        glm::vec3 v1, v2, v3;
        //for every vertex initialize a normal to 0
        for (int j = 0; j < shapes[s].mesh.positions.size()/3; j++) {
            norBuf.push_back(0);
            norBuf.push_back(0);
            norBuf.push_back(0);
        }
        // DO work here to compute the normals for every face
        //then add its normal to its associated vertex
        for (int i = 0; i < shapes[s].mesh.indices.size()/3; i++) {
            idx1 = shapes[s].mesh.indices[3*i+0];
            idx2 = shapes[s].mesh.indices[3*i+1];
            idx3 = shapes[s].mesh.indices[3*i+2];
            v1 = glm::vec3(shapes[s].mesh.positions[3*idx1 +0],shapes[s].mesh.positions[3*idx1 +1], shapes[s].mesh.positions[3*idx1 +2]);
            v2 = glm::vec3(shapes[s].mesh.positions[3*idx2 +0],shapes[s].mesh.positions[3*idx2 +1], shapes[s].mesh.positions[3*idx2 +2]);
            v3 = glm::vec3(shapes[s].mesh.positions[3*idx3 +0],shapes[s].mesh.positions[3*idx3 +1], shapes[s].mesh.positions[3*idx3 +2]);
            
            glm::vec3 u, v;
            
            u = v2 - v1;
            v = v3 - v1;
            norBuf[3*idx1+0] += u.y * v.z - u.z * v.y;
            norBuf[3*idx1+1] += u.z * v.x - u.x * v.z;
            norBuf[3*idx1+2] += u.x * v.y - u.y * v.x;
            
            u = v3 - v2;
            v = v1 - v2;
            norBuf[3*idx2+0] += u.y * v.z - u.z * v.y;
            norBuf[3*idx2+1] += u.z * v.x - u.x * v.z;
            norBuf[3*idx2+2] += u.x * v.y - u.y * v.x;
            
            u = v1 - v3;
            v = v2 - v3;
            norBuf[3*idx3+0] += u.y * v.z - u.z * v.y;
            norBuf[3*idx3+1] += u.z * v.x - u.x * v.z;
            norBuf[3*idx3+2] += u.x * v.y - u.y * v.x;
        }
        
        Renderer *renderer = Program3D->create();
        
        const vector<unsigned int> &indBuf = shapes[s].mesh.indices;
        renderer->setNumElements(indBuf.size());
        renderer->bufferData(INDICES_BUFFER, indBuf.size(), (void *)&indBuf[0]);
        
        renderer->bufferData(VERTEX_BUFFER, posBuf.size(), (void *)&posBuf[0]);
        renderer->bufferData(NORMAL_BUFFER, norBuf.size(), (void *)&norBuf[0]);
    
        renderers.push_back(renderer);
    }
}

Renderer *Entity::getRenderer(int num) {
    return renderers[num];
}

void Entity::addChild(Entity *e) {
    children.push_back(e);
}

void Entity::removeChild(Entity *e) {
    std::vector<Entity *>::iterator iChild;
    for (iChild = children.begin(); iChild < children.end(); iChild ++) {
        if (*iChild == e) {
            children.erase(iChild);
            return;
        }
    }
}

void Entity::transformBefore(glm::mat4 mat) {
    Model *= mat;
}

void Entity::transformAfter(glm::mat4 mat) {
    Model = mat * Model;
}

void Entity::update() {
    
}

void Entity::render() {
    std::vector<Renderer *>::iterator renderer;
    for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
        (*renderer)->render(Model);
    
    Renderer::pushMatrix(Model);
    
    std::vector<Entity *>::iterator child;
    for (child = children.begin(); child != children.end(); child ++)
        (*child)->render();

    Renderer::popMatrix();
}