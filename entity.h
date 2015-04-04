//
//  entity.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__entity__
#define __FinalProject__entity__

#include "tiny_obj_loader.h"
#include "renderer.h"

class Entity {
private:
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    unsigned int posBufObj, norBufObj, indBufObj;
    
protected:
    std::vector<Renderer *> renderers;
    std::vector<Entity *> children;
    
    glm::mat4 Model;
    
public:
    Entity();
    
    void load(const char *filename);
    
    void addChild(Entity *e);
    void removeChild(Entity *e);
    
    glm::mat4 getModel() { return Model; }
    
    void transformAfter(glm::mat4 mat);
    void transformBefore(glm::mat4 mat);
    void update();
    void render();
    
    Renderer *getRenderer(int num);
};

#endif /* defined(__FinalProject__entity__) */
