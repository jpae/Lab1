//
// seriously modified ZJ Wood January 2015 - conversion to glfw
// inclusion of matrix stack Feb. 2015
// original from Shinjiro Sueda
// October, 2014
//
// Base code for program 3 - does not compile as is, needs other files
//and shaders but has skeleton for much of the data transfer for shading
//and traversal of the mesh for computing normals - you must compute normals

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>

#include "main.h"
#include "world.h"
#include "camera.h"
#include "GLSL.h"
#include "tiny_obj_loader.h"
using namespace std;

#define FRAMES_PER_SEC 60
#define SEC_PER_FRAME 1 / FRAMES_PER_SEC

World *world = NULL;
GLFWwindow* window;

const int w_width = 1024;
const int w_height = 768;
const char *w_title = "Roller Coaster Funtime";

bool keysDown[GLFW_KEY_LAST] = {0};
void key_callback(GLFWwindow *window, int key, int scancode, int action, int modes) {
    if (action == GLFW_PRESS) {
        keysDown[key] = 1;
    
        if (key == GLFW_KEY_SPACE && world != NULL)
            world->switchCamera();
    }
    if (action == GLFW_RELEASE)
        keysDown[key] = 0;
}

int track_num = 1;

#define CAMERA_SPEED 0.005
#define CAMERA_MOVE 0.25
int main(int argc, char **argv) {
    // Initialise GLFW
    if(!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow(w_width, w_height, w_title, NULL, NULL);
    if(window == NULL) {
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, w_width / 2, w_height / 2);
    glfwSetKeyCallback(window, key_callback);
    
    glEnable (GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.30f, 0.5f, 0.78f, 1.0f);
    
    shaders_init();
    
    world = new World(TRACK);
    
    double clock = glfwGetTime();
    do{
        if (keysDown[GLFW_KEY_P]) {
            track_num = (track_num + 1) % 3;
            world->~World();
            if (track_num == 0)
                world = new World("tracks/track1.trk");
            if (track_num == 1)
                world = new World("tracks/track2.trk");
            if (track_num == 2)
                world = new World("tracks/track3.trk");
        }
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double nextTime = glfwGetTime();
        if (nextTime - clock > SEC_PER_FRAME) {
            // Update camera
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            double dx = (double) w_width / 2 - xpos;
            double dy = (double) w_height / 2 - ypos;
            // Edge case: window initialization
            if (xpos != 0 || ypos != 0) {
                camera_movePitch(dy * CAMERA_SPEED);
                camera_moveYaw(dx * CAMERA_SPEED);
            }
            glfwSetCursorPos(window, w_width / 2, w_height / 2);
            
            // Update camera position
            glm::vec3 cam_d(0, 0, 0);
            if (keysDown[GLFW_KEY_D])
                cam_d.x += CAMERA_MOVE;
            if (keysDown[GLFW_KEY_A])
                cam_d.x -= CAMERA_MOVE;
            if (keysDown[GLFW_KEY_W])
                cam_d.z += CAMERA_MOVE;
            if (keysDown[GLFW_KEY_S])
                cam_d.z -= CAMERA_MOVE;
            if (keysDown[GLFW_KEY_E])
                cam_d.y += CAMERA_MOVE;
            if (keysDown[GLFW_KEY_Q])
                cam_d.y -= CAMERA_MOVE;
            camera_move(cam_d.x, cam_d.y, cam_d.z);
            
            // Update and render the game
            world->update();
            
            clock = nextTime;
        }
        world->render();
        assert(glGetError() == GL_NO_ERROR);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
    out << "\n{\t";
    for(int i = 0; i < 4; i ++) {
        for(int j = 0; j < 4; j ++) {
            if (mat[j][i] == 0)
                out << "0\t";
            else
                out << mat[j][i] << "\t";
        }
        if(i < 3)
            out << std::endl << "\t";
    }
    out << "}";
    return out;
}

//Given a vector of shapes which has already been read from an obj file
// resize all vertices to the range [-1, 1]
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;
    
    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;
    
    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];
            
            if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];
            
            if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
        }
    }
    //From min and max compute necessary scale and shift for each dimension
    float maxExtent, xExtent, yExtent, zExtent;
    xExtent = maxX-minX;
    yExtent = maxY-minY;
    zExtent = maxZ-minZ;
    if (xExtent >= yExtent && xExtent >= zExtent) {
        maxExtent = xExtent;
    }
    if (yExtent >= xExtent && yExtent >= zExtent) {
        maxExtent = yExtent;
    }
    if (zExtent >= xExtent && zExtent >= yExtent) {
        maxExtent = zExtent;
    }
    scaleX = 1.0 / maxExtent;
    shiftX = minX + (xExtent/ 2.0);
    scaleY = 1.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 1.0/ maxExtent;
    shiftZ = minZ + (zExtent)/2.0;
    
    //Go through all verticies shift and scale them
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
            assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
            assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ + 0.5;
            assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
        }
    }
}