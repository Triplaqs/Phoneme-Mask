//OpenGL et rendering
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <ostream>
//outils pour la géométrie c++
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//autre
#include <random>
#include <cstdlib>
#include <ctime>
//headers
#include "vertex.h"
#include "animation.h"


// État de la dilatation (accumulation)
float currentScale = 1.0f;
// État de la température (accumulation)
float currentHeat = 0.0f;  // -1.0 (bleu froid) à +1.0 (rouge chaud)


//GESTION FENETRE
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


//GESTION INPUTS
//gestion input clavier : ici, si KEY_ESCAPE préssée
void processInput(GLFWwindow *window, bool* moveRight, bool* moveLeft, bool* moveUp, bool* moveDown){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    *moveRight = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    *moveLeft = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    *moveUp = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    *moveDown = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
}

//Edit de rendering
// Fonction pour éditer la position du triangle via uniform (exemple avec translation matrix)
void setTrianglePosition(unsigned int shaderProgram, float x, float y, float z = 0.0f, float w = 1.0f) {
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

int main(int argc, char* argv[]){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //
    
    
    //Création objet fenêtre
    GLFWwindow* window = glfwCreateWindow(800, 600, "Phoneme Mouth", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Initialisation de GLAD 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Permettre de régler gl_PointSize depuis le vertex shader
    glEnable(GL_PROGRAM_POINT_SIZE);
    //Pour la profondeur
    glEnable(GL_DEPTH_TEST);
    
    //position et dimensions de la fenêtre
    glViewport(0, 0, 800, 600);

    //possibililté de resize 
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    //préciser que l'ont veut qu'il resize régulièrement
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

/*Là c'est l'init du triangle du tuto
    //Création d'objets :
    //création 3 points :

    //Genère un buffer et créé son l'ID
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //associe le buffer à la carte graphique 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //ajout du triangle
    //Associe les modifications de buffer à celui qu'on a créé (c'est le buffer de référence)
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Idem pour le visage
    glBufferData(GL_ARRAY_BUFFER, sizeof(neutre), neutre, GL_STATIC_DRAW);

    //paramètres :
    //GL_STREAM_DRAW : La donnée est fixée une seule fois et peu utilisée par le GPU
    //GL_STATIC_DRAW : La donnée est fixée une seule fois et utilisée plusieurs fois
    //GL_DYNAMIC_DRAW : la donnée est changée et utilisée plusieurs fois
    */

    //On remplace 
    
//DEF DES SHADERS
    //Def de Shader (basique) à travers un C String
    const char *faceShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform vec4 u_centroid;\n" // centre autour duquel on scale
    "uniform float u_scale;  \n" // 1.05 ou 0.95
    "uniform vec4 offset; \n"    
    "void main()\n"
    "{\n"
    "vec4 centered = vec4(aPos, 1.0) - u_centroid;\n"
    "vec4 scaled = u_centroid + u_scale * centered;\n"
    "gl_Position = scaled + offset;\n"
    "gl_PointSize = 6.0; \n"
    "}\0";
    
    /*const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform vec4 u_centroid;\n" // centre autour duquel on scale
    "uniform float u_scale;  \n" // 1.05 ou 0.95
    "uniform vec4 offset; \n"    
    "void main()\n"
    "{\n"
    "vec4 centered = vec4(aPos, 1.0) - u_centroid;\n"
    "vec4 scaled = u_centroid + u_scale * centered;\n"
    " gl_Position = scaled + offset;\n"
    "}\0";*/

    /*const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform vec3 u_min;\n"
    "uniform vec3 u_max;\n"
    "void main()\n"
    "{\n"
    "vec3 pos = (aPos - u_min) / (u_max - u_min);\n"
    "pos = pos * 2.0 - 1.0;\n"
    "gl_Position = vec4(pos, 1.0);\n"
    "}\0";
    
    "float z = aPos.z * 0.5;\n"
    "gl_Position = vec4(aPos.x * 0.2, aPos.y * 0.2, z, 1.0);\n"
    "}\0";*/

    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    //"layout (location = 1) in vec3 aNormal;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n" 
    "uniform mat4 projection;\n"
    "out vec3 Normal;\n"
    "out vec3 FragPos;\n"    
    "void main()\n"
    "{\n"
    "FragPos = vec3(model * vec4(aPos, 1.0));\n"
    //"Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "Normal = vec3(0.0, 0.0, 1.0);\n"
    "gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\0";


    //création objet Shader
    unsigned int faceShader;
    faceShader = glCreateShader(GL_VERTEX_SHADER); 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // -> type de shader

    

    //Association de l'objet et de notre shader
    glShaderSource(faceShader, 1, &faceShaderSource, NULL);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    //compilation
    glCompileShader(faceShader);
    glCompileShader(vertexShader);

    //Idem que le vertex shader mais avec la couleur
    //Def de Shader 
    //OLD
    /*
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 color = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
    "void main()\n"
    "{\n"
    " FragColor = color;\n"
    "}\0";*/
//vec4(1.0f, 0.2f, 0.2f, 1.0f)
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 Normal;\n"
    "in vec3 FragPos;\n"
    "void main() {\n"
    // Lumière basique
    "vec3 lightPos = vec3(1.0, 1.0, 2.0);\n"
    "vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
    "vec3 objectColor = vec3(0.5, 0.2, 0.2);\n" // Rouge brique Blender
    // Ambiant
    "float ambientStrength = 0.3;\n"
    "vec3 ambient = ambientStrength * lightColor;\n"
    "vec3 norm = normalize(Normal);\n"
    "vec3 lightDir = normalize(lightPos - FragPos);\n"
    "float diff = max(dot(norm, lightDir), 0.0);\n"
    "vec3 diffuse = diff * lightColor;\n"
    "vec3 result = (ambient + diffuse) * objectColor;\n"
    "FragColor = vec4(result, 1.0);\n"
    "}\0";   
//HEY REPRENDS LA FINIS DE SET LES FRAGMENTS SHADERS !!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//création objet Shader
    unsigned int fragmentShaderFace;
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // -> type de shader
    //Association de l'objet et de notre shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //compilation
    glCompileShader(fragmentShader);

    //Creer objet programme
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //attache les objets au programme
    //glAttachShader(shaderProgram, faceShader);
    //essayons d'en garder qu'un seul
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //appel au programme 
    glUseProgram(shaderProgram);

    //On supprime les objets après les avoir attaché
    
    glDeleteShader(faceShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //On précise à OpenGL comment interpréter nos données pour les afficher
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),  (void*)0);
    glEnableVertexAttribArray(0);

    /*
//On remplace ici aussi vertices (le triangle) par neutre (le visage)
//Manipulation d'objet sans structure :
    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neutre), neutre, GL_STATIC_DRAW);
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
    glEnableVertexAttribArray(0);
    // 2. use our shader program when we want to render an object
    glUseProgram(shaderProgram);
    // 3. now draw the object
    //someOpenGLFunctionThatDrawsOurTriangle();

//Manipulation d'objet avec structure :
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //avec triangle
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // utiliser neutre (nuage 3D)
    glBufferData(GL_ARRAY_BUFFER, sizeof(neutre), neutre, GL_STATIC_DRAW);
 
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
    glEnableVertexAttribArray(0);
    // Drawing code (in render loop) :: ..
    // 4. draw the object
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //someOpenGLFunctionThatDrawsOurTriangle();
    */

    GLuint VBO, EBO, VAO;
    //Attribution des vertices avec indices cette fois
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neutre), neutre, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //pour le dessin
    int indexCount = sizeof(indices) / sizeof(unsigned int);

    // Initialiser les uniforms du dilate triangle
    /*float cx = (vertices[0] + vertices[3] + vertices[6]) / 3.0f;
    float cy = (vertices[1] + vertices[4] + vertices[7]) / 3.0f;
    float cz = (vertices[2] + vertices[5] + vertices[8]) / 3.0f;
    GLint loc_centroid = glGetUniformLocation(shaderProgram, "u_centroid");
    GLint loc_scale = glGetUniformLocation(shaderProgram, "u_scale");
    glUniform4f(loc_centroid, cx, cy, cz, 1.0f);
    glUniform1f(loc_scale, 1.0f);*/

    //idem pour le visage
    // Initialiser les uniforms du modèle à partir de neutre[]
    int numVerts = (int)((sizeof(neutre) / sizeof(float)) / 3);

    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    float minx = 1e9f, miny = 1e9f, minz = 1e9f;
    float maxx = -1e9f, maxy = -1e9f, maxz = -1e9f;
    for (int i = 0; i < numVerts; ++i) {
        float x = neutre[3*i + 0];
        float y = neutre[3*i + 1];
        float z = neutre[3*i + 2];
        cx += x; cy += y; cz += z;
        minx = std::min(minx, x); miny = std::min(miny, y); minz = std::min(minz, z);
        maxx = std::max(maxx, x); maxy = std::max(maxy, y); maxz = std::max(maxz, z);
    }
    cx /= numVerts; cy /= numVerts; cz /= numVerts;
    float rangeX = maxx - minx;
    float rangeY = maxy - miny;
    float rangeZ = maxz - minz;
    float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));
    float fitScale = (maxRange > 0.0f) ? (2.0f / maxRange) : 1.0f; // mappe la plus grande étendue sur [-1,1]

    GLint loc_centroid = glGetUniformLocation(shaderProgram, "u_centroid");
    GLint loc_scale = glGetUniformLocation(shaderProgram, "u_scale");
    glUniform4f(loc_centroid, cx, cy, cz, 1.0f);
    glUniform1f(loc_scale, fitScale * 0.9f); // petit padding


//render loop (maintient la fenêtre ouverte, une loop = une frame)
    //se divise en 4 parties : nettoyage, input, render puis cloture
    while(!glfwWindowShouldClose(window)){
    //P1 : nettoyage
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Aussi GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
        
        
    //P2 : gestion input clavier
        bool moveRight = false;
        bool moveLeft = false;
        bool moveUp = false;
        bool moveDown = false;
        processInput(window, &moveRight, &moveLeft, &moveUp, &moveDown);

    //P3 : gestion du render
        //Attention : au choix du programme Shader utilisé

        //dessin du triangle
        glUseProgram(shaderProgram);
        // Dans la boucle de rendu
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(fitScale)); // Utilise ton fitScale calculé
        model = glm::translate(model, glm::vec3(-cx, -cy, -cz)); // Centre le masque
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Envoie-les au shader
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //glUniform3f(loc_min, minx, miny, minz);
        //glUniform3f(loc_max, maxx, maxy, maxz);
        glBindVertexArray(VAO);


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //essaye GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        
        
        // Accumuler la dilatation et la température
        if(moveUp){    
            //future interaction clavier
        }
        if(moveDown){    
            //future interaction clavier
        }
        
        // Réinitialiser les uniforms à chaque frame
        /*float cx = (vertices[0] + vertices[3] + vertices[6]) / 3.0f;
            currentScale *= 1.01f;  // Augmenter de 1% par frame
            heatTriangle(shaderProgram, -0.01f);  // Augmenter la température
        }
        if(moveDown){    
            currentScale *= 0.99f;  // Diminuer de 1% par frame
            heatTriangle(shaderProgram, 0.01f);  // Diminuer la température
        }
        
        // Réinitialiser les uniforms à chaque frame
        float cx = (vertices[0] + vertices[3] + vertices[6]) / 3.0f;
        float cy = (vertices[1] + vertices[4] + vertices[7]) / 3.0f;
        float cz = (vertices[2] + vertices[5] + vertices[8]) / 3.0f;
        GLint loc_centroid = glGetUniformLocation(shaderProgram, "u_centroid");
        GLint loc_scale = glGetUniformLocation(shaderProgram, "u_scale");
        GLint loc_offset = glGetUniformLocation(shaderProgram, "offset");
        glUniform4f(loc_centroid, cx, cy, cz, 1.0f);
        glUniform1f(loc_scale, currentScale);
        glUniform4f(loc_offset, 0.0f, 0.0f, 0.0f, 0.0f); */
        
        if(moveRight){    
            setTriangleColorRand(shaderProgram);
        }
        if(moveLeft){    
            currentScale = 1.0f;  // Annuler la dilatation quand on tourne
            makeTriangleSpin(shaderProgram, (float)glfwGetTime());
        }
        //dessin triangle
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //dessin visage
        //glDrawArrays(GL_POINTS, 0, numVerts);
        
    //P4 : fin render loop
        //met les pixels en couleur
        glfwSwapBuffers(window);
        //vérifie si un input a été trigger
        glfwPollEvents();
    }

    printf("fenêtre Phoneme Mouth fermée\n");
    glfwTerminate();
    return 0;
}

