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
#include <algorithm>
#include <iterator>
//headers
#include "vertex.h"
#include "animation.h"

//Anciennes variables pour shaders
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
void processInput(GLFWwindow *window, bool* moveRight, bool* moveLeft, bool* moveUp, bool* moveDown, bool* pressR){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    *moveRight = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    *moveLeft = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    *moveUp = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    *moveDown = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
    *pressR = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
}

int main(int argc, char* argv[]){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
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

    
//DEF DES SHADERS
    //Def de Shader (basique) à travers un C String
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
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // -> type de shader

    

    //Association de l'objet et de notre shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    //compilation
    glCompileShader(vertexShader);

    //Idem que le vertex shader mais avec la couleur
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 Normal;\n"
    "in vec3 FragPos;\n"
    //main
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
    //essai profondeur
    "float depth = (FragPos.z + 0.3);\n"
    "FragColor = vec4(vec3(depth), 1.0);\n"  //ombre
    //"FragColor = vec4(result, 1.0);\n"  //couleur
    "}\0";   

//HEY REPRENDS LA FINIS DE SET LES FRAGMENTS SHADERS !!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//création objet Shader
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
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //On précise à OpenGL comment interpréter nos données pour les afficher
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),  (void*)0);
    glEnableVertexAttribArray(0);

    //on initialise notre visage au neutre
    //std::vector<float> face(3*n);
    face.resize(3*n);
    std::copy(neutre, neutre + 3*n, face.begin());

    GLuint VBO, EBO, VAO;
    //Attribution des vertices aux objets (on tient compte de quels vertices forment des arrêtes avec quelles vertices)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //.data() pour avoir les données comme float*
    glBufferData(GL_ARRAY_BUFFER, face.size() * sizeof(float), face.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //pour le dessin
    int indexCount = sizeof(indices) / sizeof(unsigned int);

    
    //idem pour le visage
    // Initialiser les uniforms du modèle à partir de face[] (=neutre ici)
    //int numVerts = (int)((sizeof(face) / sizeof(float)) / 3);  
    //int numVerts = face.size() / 3;
    //plus besoin numverts = n (global) 

    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    float minx = 1e9f, miny = 1e9f, minz = 1e9f;
    float maxx = -1e9f, maxy = -1e9f, maxz = -1e9f;
    for (int i = 0; i < n; ++i) {
        float x = face[3*i + 0];
        float y = face[3*i + 1];
        float z = face[3*i + 2];
        cx += x; cy += y; cz += z;
        minx = std::min(minx, x); miny = std::min(miny, y); minz = std::min(minz, z);
        maxx = std::max(maxx, x); maxy = std::max(maxy, y); maxz = std::max(maxz, z);
    }
    cx /= n; cy /= n; cz /= n;
    float rangeX = maxx - minx;
    float rangeY = maxy - miny;
    float rangeZ = maxz - minz;
    float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));
    float fitScale = (maxRange > 0.0f) ? (2.0f / maxRange) : 1.0f; // mappe la plus grande étendue sur [-1,1]

    GLint loc_centroid = glGetUniformLocation(shaderProgram, "u_centroid");
    GLint loc_scale = glGetUniformLocation(shaderProgram, "u_scale");
    glUniform4f(loc_centroid, cx, cy, cz, 1.0f);
    glUniform1f(loc_scale, fitScale * 0.9f); // petit padding

    //printf("%d", std::equal(neutre, neutre + 3*n, phoneme_A));


//render loop (maintient la fenêtre ouverte, une loop = une frame)
    //se divise en 4 parties : nettoyage, input, render puis cloture
    while(!glfwWindowShouldClose(window)){
        //mesure du temps pour animation
        float currentTime = glfwGetTime();
    //P1 : nettoyage
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Aussi GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
        
        
    //P2 : gestion input clavier
        bool moveRight = false;
        bool moveLeft = false;
        bool moveUp = false;
        bool moveDown = false;
        bool pressR = false;
        processInput(window, &moveRight, &moveLeft, &moveUp, &moveDown, &pressR);

    //P3 : gestion du render
        //Attention : au choix du programme Shader utilisé

        //réaction inputs
        if(moveUp){
            camera.viewz += 0.05;
        }
        if(moveDown){   
            camera.viewz -= 0.05; 
        }
        if(pressR){   
            camera.reset(); 
        }

        //dessin du triangle
        glUseProgram(shaderProgram);
        
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //rotation car le masque est vers le haut dans Blender
        //centrer, tourner, scale (ordre)
        model = glm::scale(model, glm::vec3(fitScale*0.5f)); // Utilise fitScale calculé 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //axe de profil
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //flip le masque
        model = glm::translate(model, glm::vec3(-cx, -cy, -cz)); // Centre le masque
        glm::mat4 view = glm::lookAt(glm::vec3(camera.viewx, camera.viewy, camera.viewz), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //position de la cam, vers où elle regarde, up vecteur
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Envoie au shader
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
        
        if(moveRight){    
            if(animStartTmps < 0.0f){
                switchTo(1, currentTime);
            }   
        }
        if(moveLeft){
            if(animStartTmps < 0.0f){
                switchTo(0, currentTime);
            }     
        }

        //Animation (= interpolation des phonèmes)
        if(animStartTmps >= 0.0f){
            //calcul de t
            t = (currentTime - animStartTmps) / animDuree;
            if(t >= 1.0f){
                t = 1.0f;
                animStartTmps = -1.0f; //fin de l'animation
                //mettre à jour l'état du visage
                //facestruct.etat = (facestruct.etat == 0) ? 1 : 0;
                switchedTo(facestruct.next_etat);
            }
            //interpolation des vertices
            const float * phoneme1 = getPhoneme(facestruct.etat);
            const float * phoneme2 = getPhoneme(facestruct.next_etat);
            for(int i = 0; i < 3*n; i++){
                face[i] = (1.0f - t) * phoneme1[i] + t * phoneme2[i];
            }
            //mettre à jour le buffer des vertices
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, face.size() * sizeof(float), face.data());
        }
        
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
