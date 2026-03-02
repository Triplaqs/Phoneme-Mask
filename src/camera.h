#ifndef CAMERA_H
#define CAMERA_H

//outils pour la géométrie c++
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Struct Caméra = variables de caméras globales
typedef struct Camera {
    float viewx = 0.0f;
    float viewy = 0.0f;
    float viewz = 2.0f;
    //reset la position de la caméra 
    float lastX; // Milieu de la fenêtre X
    float lastY; // Milieu de la fenêtre Y
    float yaw;  // Rotation horizontale
    float pitch;  // Rotation verticale
    bool firstMouse;
    float fov;
    glm::vec3 cameraTarget;
    float cameraDistance;
        void reset() {
        viewx = 0.0f;
        viewy = 0.0f;
        viewz = 2.0f;
        lastX = 400;
        lastY = 300;
        yaw = -90.0f;
        pitch = 0.0f;
        firstMouse = true;
        fov = 45.0f;
        cameraTarget = glm::vec3(0.0f, -0.18f, 0.0f);
        cameraDistance = 3.0f;
    }
} Camera;

extern Camera camera;  


//fonctions :

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif