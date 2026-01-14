//OpenGL et rendering
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//Autres fichiers
#include "vertex.h"
#include "animation.h"
//autre
#include <random>
#include <cstdlib>
#include <ctime>

//constantes d'animations
float animStartTmps = -1.0f; //-1 si rien en cours
float animDuree = 0.5f; //durée d'une animation en secondes
float t = 0.0f; //temps pour interpolation


//Fonctions d'animation

//Prends un indice i et renvoie le phonème associé (voir attribution Annexe en dessous)
const float* getPhoneme(int i){
    return phonemes[i];
}

//switch to phoneme i (set up la transition)
void switchTo(int i, float currentTime){
    //set up des variables globales d'animation
    animStartTmps = currentTime; //démarre l'animation
    //t = 0.0f; //re défini à chaque fois
    facestruct.next_etat = i;
}


//Edit de rendering
// Fonction pour éditer la position du triangle via uniform (exemple avec translation matrix)
void setTrianglePosition(unsigned int shaderProgram, float x, float y, float z, float w) { //predéfinitions dans le headers
    glUseProgram(shaderProgram);
    int posLoc = glGetUniformLocation(shaderProgram, "offset");
    glUniform4f(posLoc, x, y, z, w);
}

// Fonction pour éditer la couleur du triangle via uniform
void setTriangleColor(unsigned int shaderProgram, float r, float g, float b, float a) {
    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLoc, r, g, b, a);
}

void generateFace(){
    //Peut être pour une autre fois
}


void setTriangleColorRand(unsigned int shaderProgram) {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float a = 1.0f;
    setTriangleColor(shaderProgram, r, g, b, a);
}

void makeTriangleSpin(unsigned int shaderProgram, float time) {
    float angle = time;
    //float angle = (float)glfwGetTime();
    float x = 0.5f * cos(angle);
    float y = 0.5f * sin(angle);
    setTrianglePosition(shaderProgram, x, y);
}