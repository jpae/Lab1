Thomas Steinke & Jonathan Pae

CPE 476 - Lab 1

-------------

# Debugging!

## Geometric Primitives

GL Primitive structures are super useful for debugging, especially when you need to visualize bounding boxes, important object shapes, etc. BUT the interface is *TOTALLY* different from the rest of your rendering pipeline (and inefficient as well). Here are your key pieces:

### Setting up models
```
glMatrixMode(/* GL_PROJECTION, GL_MODELVIEW */); // This sets up either the Projection or ModelView matrix to be edited

glPushMatrix();
glPopMatrix();
glLoadIdentity(); // Set the top of the matrix to the equivalent of glm::mat4(1)
glLoadMatrixf(float *M); // Load a matrix onto the top of the matrix stack
glMultMatrix(float *M); // Multiply the top of the matrix stack by M
```

### Rendering
```
glBegin(/* GL_TRIANGLES, GL_LINES, GL_QUADS, ... */);
/* Draw stuff here */
glEnd()
```

### Examples

Setting up a projection matrix

```
glm::mat4 Projection = glm::perspective(45.0f, (float) w_width / w_height, 0.01f, 400.0f);

glMatrixMode(GL_PROJECTION); // Set up Projection matrix for changing
glLoadMatrixf(&Projection[0][0]); // Load in the projection matrix from your game

glMatrixMode(GL_MODELVIEW); // Set up ModelView matrix for editing
glLoadIdentity(); // Blank slate
```

Drawing a line

```
glMultMatrix(&Model[0][0]); // Add Model's matrix in the stack
glBegin(GL_LINES);
   glVertex3f(-1, 0, 0);
   glVertex3f( 1, 0, 0);
glEnd();
```
That draws a line across the middle of the model.

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

## GameObject

Game Objects are mostly just a collection of components. There's a small interface for working with them though:

```
float gameObject->getX(), gameObject->getY(), gameObject->getZ();
float gameObject->getRadius(); // Get the radius of the bounding sphere
float gameObject->getSpeed();
glm::vec3 gameObject->getDirection();

void gameObject->setX(float x), gameObject->setY(float y), gameObject->setZ(float z);
void gameObject->setSpeed(float speed);
void gameObject->setDirection(glm::vec3 direction);

void gameObject->collide(GameObject *other); // Test (and possibly react to) collision between gameObject and other
void gameObject->update(World *world, float dt); // Update the object
void gameObject->render();
```

## Components

Components are the bread and butter of GameObjects. This is where all of their behavior (input, physics, collision, etc) is defined and changed.

### Graphics Components

Graphics components deal with rendering GameObjects. Each GraphicComponent has a field ```renderers```, which are objects comingfrom the Renderers section.

```render(GameObject *obj)``` usually isn't necessary to call. Instead, create all of the renderers and add them to the ```renderers``` vector in the Component's constructor

### Physics Components

Physics components deal with world interactions, as well as time-related (aka physics) stuff.

```update(GameObject *obj, World *world, float dt)``` is where all of your computation is uaully done.

### Collision Components

Collision Components handle object collision. What's important is that if you want an object to be collideable, at least add `CollisionComponent()`  to it.

```collide(GameObject *obj, GameObject *other)``` is called whenever this object collides with another object. Use this for reactionary effects (setting remove to true, moving object, etc).

### Component

This is the most general component (Usually for input). It has a relatively simple ```update(GameObject *obj)``` call.

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

