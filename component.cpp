//
//  component.cpp
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//
#include <vector>
#include <iostream>
#include <string>
#include <glm/ext.hpp>
#include "GLSL.h"
#include "main.h"
#include "world.h"
#include "component.h"
#include "gameobject.h"

const float PLAYER_SPEED = 0.25f;

/* Movement Component */
void MovementComponent::update(GameObject *obj, World *world, float dt) {
    float world_speed = obj->getSpeed() / FRAMES_PER_SEC;

    //Check for the edge of the platforms
    if (fabs(obj->getX()) > GROUND_WIDTH/2 || fabs(obj->getZ()) > GROUND_WIDTH/2) {
        //Find a new direction for the object
        glm::vec3 newDirectionPoint = randPoint(GROUND_WIDTH/3);
        obj->setDirection(newDirectionPoint - glm::vec3(obj->getX(), 0, obj->getZ()));
    }

    obj->setZ(obj->getZ() + world_speed * obj->getDirection().z);
    obj->setX(obj->getX() + world_speed * obj->getDirection().x);
}

/* Player Input Component */
void PlayerInputComponent::update(GameObject *obj) {
    if (keysDown[GLFW_KEY_J]) {
        obj->setX(obj->getX() - PLAYER_SPEED); 
    }
    if (keysDown[GLFW_KEY_K]) {
        obj->setZ(obj->getZ() + PLAYER_SPEED);
    }
    if (keysDown[GLFW_KEY_L]) {
        obj->setX(obj->getX() + PLAYER_SPEED); 
    }
    if (keysDown[GLFW_KEY_I]) {
        obj->setZ(obj->getZ() - PLAYER_SPEED);
    }
}

/* Collision components */
void TargetCollisionComponent::collide(GameObject *obj, GameObject *other) {
    float dx = obj->getX() - other->getX();
    float dz = obj->getZ() - other->getZ();

    obj->setDirection(glm::vec3(dx, 0, dz));
}

void PlayerCollisionComponent::collide(GameObject *obj, GameObject *other) {
    other->setCollision(NULL);
    other->setPhysics(NULL);
    other->getGraphics()->getRenderer(0)->mat = MATERIAL_GRASS;

    score ++;
}

/* Graphics Renderers */
GraphicsComponent::GraphicsComponent() {
    renderers.clear();
    bounds.min_x = bounds.max_x = 0;
    bounds.min_y = bounds.max_y = 0;
    bounds.min_z = bounds.max_z = 0;
}

void GraphicsComponent::setBounds(GameObject *obj)  { 
    memcpy(&obj->bounds, &bounds, sizeof(Bounds));
}

void GraphicsComponent::render(GameObject *obj) {
    glm::mat4 Model = obj->getModel();

    std::vector<Renderer *>::iterator renderer;
    for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
        (*renderer)->render(Model);
}

GroundRenderer::GroundRenderer(float size) {
    GraphicsComponent::GraphicsComponent();

    bounds.min_x = bounds.min_z = -size;
    bounds.max_x = bounds.max_z =  size;
    bounds.min_y = -1;
    bounds.max_y = 0;

    const float posBuf[] = {
        -size, 0,  size,
         size, 0,  size,
        -size, 0, -size,
         size, 0, -size
    };

    const unsigned int indBuf[] = {
        0, 1, 2, 1, 2, 3
    };

    const float norBuf[] = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0
    };
        
    Renderer *renderer = Program3D->create();
    
    renderer->setNumElements(6);
    renderer->bufferData(INDICES_BUFFER, 6, (void *)&indBuf[0]);
    
    renderer->bufferData(VERTEX_BUFFER, 4 * 3, (void *)&posBuf[0]);
    renderer->bufferData(NORMAL_BUFFER, 4 * 3, (void *)&norBuf[0]);

    renderers.push_back(renderer);
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

    // Compute bounds
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < bounds.min_x) bounds.min_x = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > bounds.max_x) bounds.max_x = shapes[i].mesh.positions[3*v+0];
            
            if(shapes[i].mesh.positions[3*v+1] < bounds.min_y) bounds.min_y = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > bounds.max_y) bounds.max_y = shapes[i].mesh.positions[3*v+1];
            
            if(shapes[i].mesh.positions[3*v+2] < bounds.min_z) bounds.min_z = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > bounds.max_z) bounds.max_z = shapes[i].mesh.positions[3*v+2];
        }
    }
}


