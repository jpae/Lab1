//
//  component.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//

#ifndef __Project__component__
#define __Project__component__

#include "renderer.h"

class World;
class GameObject;

class GraphicsComponent {
protected:
    std::vector<Renderer *> renderers;

public:
    GraphicsComponent() { renderers.clear(); };

    virtual void render(GameObject *obj);
};

class ModelRenderer : public GraphicsComponent {
public:
    ModelRenderer(const char *filename);
};

class PhysicsComponent {
public:
    virtual void update(GameObject *obj, World *world, float dt);
};

class Component {
public:
    virtual void update(GameObject *obj){};
};

class PlayerInputComponent : public Component {
public:
    void update(GameObject *obj);
};

#endif /* __Project__component__ */
