//
//  ground.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/15/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <vector>
#include "ground.h"

#define GROUND_SIZE 50
Ground::Ground() {
    std::vector<float> posBuf;
    std::vector<float> texBuf;
    std::vector<unsigned int> indBuf;
    
    const int SCALE = 4;
    for(int i = 0; i < GROUND_SIZE * GROUND_SIZE; i ++) {
        indBuf.push_back(4 * i);
        indBuf.push_back(4 * i + 1);
        indBuf.push_back(4 * i + 2);
        indBuf.push_back(4 * i);
        indBuf.push_back(4 * i + 2);
        indBuf.push_back(4 * i + 3);
        
        int x = SCALE * (i % GROUND_SIZE);
        int y = SCALE * (i / GROUND_SIZE);
        posBuf.push_back((x - GROUND_SIZE / 2) * SCALE);
        posBuf.push_back(-1);
        posBuf.push_back((y - GROUND_SIZE / 2) * SCALE);
        
        posBuf.push_back((x - GROUND_SIZE / 2) * SCALE);
        posBuf.push_back(-1);
        posBuf.push_back((y + SCALE - GROUND_SIZE / 2) * SCALE);
        
        posBuf.push_back((x + SCALE - GROUND_SIZE / 2) * SCALE);
        posBuf.push_back(-1);
        posBuf.push_back((y + SCALE - GROUND_SIZE / 2) * SCALE);
        
        posBuf.push_back((x + SCALE - GROUND_SIZE / 2) * SCALE);
        posBuf.push_back(-1);
        posBuf.push_back((y - GROUND_SIZE / 2) * SCALE);
        
        texBuf.push_back(1);
        texBuf.push_back(0);
        texBuf.push_back(0);
        texBuf.push_back(0);
        texBuf.push_back(0);
        texBuf.push_back(1);
        texBuf.push_back(1);
        texBuf.push_back(1);
    }
    
    TexRenderer *renderer = (TexRenderer *) ProgramTex->create();
    
    const char *texture = "textures/grass3.bmp";
    renderer->loadTexture((char *)texture);
    renderer->setNumElements(indBuf.size());
    renderer->bufferData(INDICES_BUFFER, indBuf.size(), &indBuf[0]);
    
    renderer->bufferData(VERTEX_BUFFER, posBuf.size(), &posBuf[0]);
    renderer->bufferData(UV_BUFFER, texBuf.size(), &texBuf[0]);
    renderers.push_back(renderer);
}
