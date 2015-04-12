//
//  component.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//

#ifndef __Project__component__
#define __Project__component__

#include "renderer.h"
#include "bounds.h"

class World;
class GameObject;

/* Graphics components render to the screen */
class GraphicsComponent {
protected:
    Bounds bounds;
    std::vector<Renderer *> renderers;

public:
    GraphicsComponent();

    Renderer *getRenderer(int i) { return renderers[i]; }
    virtual void render(GameObject *obj);
    virtual void setBounds(GameObject *obj);
};

class ModelRenderer : public GraphicsComponent {
public:
    ModelRenderer(const char *filename);
};

class GroundRenderer : public GraphicsComponent {
public:
    GroundRenderer(float size);
};

/* Physics components require a reference to the world */
class PhysicsComponent {
public:
    PhysicsComponent(){};
    ~PhysicsComponent(){};
    virtual void update(GameObject *obj, World *world, float dt) {};
};

class MovementComponent : public PhysicsComponent {
public:
    MovementComponent(){};
    ~MovementComponent(){};
    virtual void update(GameObject *obj, World *world, float dt);
};

/* Collision components specify on-collision behavior */
class CollisionComponent {
public:
    virtual void collide(GameObject *obj, GameObject *other) {};
};

class PlayerCollisionComponent : public CollisionComponent {
public:
    PlayerCollisionComponent() : score(0) {};

    int score;
    void collide(GameObject *obj, GameObject *other);
};

class TargetCollisionComponent : public CollisionComponent {
public:
    void collide(GameObject *obj, GameObject *other);
};

/* Generic components don't require access to the world */
class Component {
public:
    virtual void update(GameObject *obj){};
};

class PlayerInputComponent : public Component {
public:
    void update(GameObject *obj);
};

#endif /* __Project__component__ */
