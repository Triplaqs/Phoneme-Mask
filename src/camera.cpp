//includes GL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//includes du projet
#include "vertex.h"
#include "camera.h"
//Includes ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//outils pour la géométrie c++
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//variables globales :

//Struct Caméra
Camera camera;   


//fonctions :


//Manipulation "caméra" acex la souris :
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        camera.firstMouse = true;
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse) {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = ypos - camera.lastY; 
    camera.lastX = xpos;
    camera.lastY = ypos;

    float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        
        //Shift Slide (comem sur Blender)
        // On calcule les vecteurs haut et bas de la cam
        glm::vec3 front = glm::normalize(glm::vec3(
            cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw)),
            sin(glm::radians(camera.pitch)),
            cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw))
        ));
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));

        // On déplace la cible du pov de la cam(target)
        camera.cameraTarget += right * (xoffset * 0.005f);
        camera.cameraTarget += up * (yoffset * 0.005f);
        
    } else {
        //Déplacement rotatif (tjrs à la Blender)
        camera.yaw += xoffset;  
        camera.pitch += yoffset; 

        if (camera.pitch > 89.0f){
            camera.pitch = 89.0f;
        } 
        if (camera.pitch < -89.0f){
            camera.pitch = -89.0f;
        } 
    }
}

//scroll molette
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    camera.cameraDistance -= (float)yoffset * 0.3f; //0.5
    if (camera.cameraDistance < 0.5f) camera.cameraDistance = 0.5f;
}