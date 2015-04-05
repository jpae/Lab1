Thomas Steinke & Jonathan Pae

CPE 476 - Lab 1

-------------

# Core

## Main

This file does the initialization work (window & world creation, and manages the game loop. I have it set up so that ```world->update``` and ```world->render``` are called at a consistent 60 FPS. Most of my constants (FPS, MATH_PI and conversions, tweakable constants) are stored in ```main.h```

## World

This is where all of our actual game logic starts. Right now it's pretty simple, with just a vector of entities that's drawn each frame. To add a new entity to the world, you have two options:

Adding a top-level entity:
```
Entity myEntity = new Entity();
myEntity->load("models/something.obj");
/* Transform myEntity */
entities->push_back(myEntity);
```

Adding a relative entity (e.g. for hierarchical modeling):
```
Entity mySubEntity = new Entity();
mySubEntity->load("models/something_else.obj");
/* Transform mySubEntity */
myEntity->addChild(mySubEntity);
```

## Camera

This is a camera singleton, which manages, well, the camera. Here are possible commands:

Initialization:
```
void camera_setPosition(glm::vec3 position);
void camera_setTransform(glm::mat4 _transform);
void camera_lookAt(glm::vec3 dest);
```

Updating:
```
void camera_movePitch(double dp);
void camera_moveYaw(double dy);
void camera_move(float dx, float dy, float dz);
```

Getting information:
```
glm::mat4 camera_getMatrix();
glm::vec3 camera_getPosition();
glm::vec3 camera_getLookAt();
```

## Entity

I set this up to use the Entity-Component system. Here's the interface:

Initialization:
```
void entity->load();
void entity->addChild(Entity *e);
void entity->removeChild(Entity *e);

void entity->transformBefore(glm::mat4 mat); // The same as doing Model = Model * mat
void entity->transformAfter (glm::mat4 mat); // The same as doing Model = mat * Model
```

Called (more or less) every frame:
```
void entity->update(); // Empty by default, make a subclass
void entity->render();
```

In case you want it:
```
void entity->getModel();
```

## Renderer

You probably won't need to mess with this much, but it's our graphics powerhouse. It does GL Buffer creation, Data buffering, and deals with shaders and all that jazz. Important takeaways:

Each program (only one in this case) is a separate struct (i.e. Program3D is the basic 3D-stuff program). They all follow the same workflow:
```Renderer *myRenderer = Program3D->create();```

This Renderer object is what you use to render stuff. It's kind of like a wrapper for OpenGL functionality.
```
myRenderer->setNumElements(long numElements); // Refers to the # of indices
myRenderer->bufferData(int type, long num, void *data); // Type can be VERTEX_BUFFER, COLOR_BUFFER, NORMAL_BUFFER, INDICES_BUFFER
myRenderer->mat = MATERIAL_RUBBER; // Material constants are defined in renderer.h

myRenderer->render();
```

