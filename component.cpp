//
//  component.cpp
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//
#include <vector>
#include <iostream>
#include <string>
#include "GLSL.h"
#include "main.h"
#include "component.h"
#include "gameobject.h"

void PlayerInputComponent::update(GameObject *obj) {
    if (keysDown[GLFW_KEY_J]) {
        obj->setX(obj->getX() - 0.5); 
    }
    if (keysDown[GLFW_KEY_K]) {
        obj->setZ(obj->getZ() + 0.5);
    }
    if (keysDown[GLFW_KEY_L]) {
        obj->setX(obj->getX() + 0.5); 
    }
    if (keysDown[GLFW_KEY_I]) {
        obj->setZ(obj->getZ() - 0.5);
    }
}

void GraphicsComponent::render(GameObject *obj) {
    glm::mat4 Model = obj->getModel();


    std::vector<Renderer *>::iterator renderer;
    for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
        (*renderer)->render(Model);
}

ModelRenderer::ModelRenderer(const char *filename) {
    GraphicsComponent::GraphicsComponent();

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err = tinyobj::LoadObj(shapes, materials, filename);
    if(!err.empty()) {
        std::cerr << err << std::endl;
    }
    resize_obj(shapes);
    
    for(int s = 0; s < shapes.size(); s ++) {
        const std::vector<float> &posBuf = shapes[s].mesh.positions;
        
        std::vector<float> norBuf;
        int idx1, idx2, idx3;
        glm::vec3 v1, v2, v3;
        // For every vertex initialize a normal to 0
        for (int j = 0; j < shapes[s].mesh.positions.size()/3; j++) {
            norBuf.push_back(0);
            norBuf.push_back(0);
            norBuf.push_back(0);
        }
        // Compute the normals for every face
        // then add its normal to its associated vertex
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
        
        const std::vector<unsigned int> &indBuf = shapes[s].mesh.indices;
        renderer->setNumElements(indBuf.size());
        renderer->bufferData(INDICES_BUFFER, indBuf.size(), (void *)&indBuf[0]);
        
        renderer->bufferData(VERTEX_BUFFER, posBuf.size(), (void *)&posBuf[0]);
        renderer->bufferData(NORMAL_BUFFER, norBuf.size(), (void *)&norBuf[0]);
    
        renderers.push_back(renderer);
    }
}


