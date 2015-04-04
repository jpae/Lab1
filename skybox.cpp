//
//  skybox.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/17/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <glm/ext.hpp>
#include "skybox.h"

#define SKY_SIZE 100.0f
Skybox::Skybox() {
    unsigned int indBuf[] = {
        0, 2, 1,
        0, 3, 2,
        3, 5, 2,
        3, 4, 5,
        4, 6, 5,
        4, 7, 6,
        7, 9, 6,
        7, 8, 9,
        5, 6, 11,
        5, 11, 10,
        12, 7, 4,
        12, 13, 7,
        14, 15, 16,
        14, 17, 15
    };
    float posBuf[] = {
        -SKY_SIZE, -SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE,  SKY_SIZE, -SKY_SIZE,
         SKY_SIZE,  SKY_SIZE, -SKY_SIZE, //
         SKY_SIZE, -SKY_SIZE, -SKY_SIZE, //
         SKY_SIZE, -SKY_SIZE,  SKY_SIZE,
         SKY_SIZE,  SKY_SIZE,  SKY_SIZE,
        -SKY_SIZE,  SKY_SIZE,  SKY_SIZE,
        -SKY_SIZE, -SKY_SIZE,  SKY_SIZE,
        -SKY_SIZE, -SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE,  SKY_SIZE, -SKY_SIZE,
        
         SKY_SIZE,  SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE,  SKY_SIZE, -SKY_SIZE,
         SKY_SIZE, -SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE, -SKY_SIZE, -SKY_SIZE,
        
        SKY_SIZE, -20, -SKY_SIZE, // 12
        -SKY_SIZE, -20,  SKY_SIZE, // 7
        SKY_SIZE, -20,  SKY_SIZE, // 4
        -SKY_SIZE, -20, -SKY_SIZE, // 13
    };

    static float GrndTex[] = {
        0, 0.5,
        0, 0.25,
        0.25, 0.25,
        0.25, 0.5,
        0.5, 0.5, // 4
        0.52, 0.25,
        0.76, 0.25,
        0.76, 0.5, // 7
        1, 0.5,
        1, 0.25,
        0.52, 0,
        0.76, 0,
        0.5, 0.75,
        0.76, 0.75,
        
        0.52, 0.75,
        0.76, 0.5, // 7
        0.52, 0.5, // 4
        0.76, 0.75,
    };
    
    TexRenderer *renderer = (TexRenderer *) ProgramTex->create();
    
    const char *texture = "textures/skybox.bmp";
    renderer->loadTexture((char *)texture);
    renderer->setNumElements(36);
    renderer->bufferData(INDICES_BUFFER, 36, indBuf);
    
    renderer->bufferData(VERTEX_BUFFER, 18 * 3, posBuf);
    renderer->bufferData(UV_BUFFER, 18 * 2, GrndTex);
    renderers.push_back(renderer);
    
//    transformBefore(glm::translate(0.0f, SKY_SIZE, 0.0f));
}