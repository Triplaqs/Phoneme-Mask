#ifndef VERTEX_H
#define VERTEX_H


#include <vector>
#include <string>



//Structure pour manipuler les vertices du visage (abandonné)
typedef struct Point {
    float x, y, z;
    float w = 1.0f;
} Point;

//définition de singletons à manipuler en global
typedef struct Face {
    std::vector<float> points; //liste des vertices
    //int nb_pts;    // nombre de vertices (plus besoin, variable globale n)
    int etat = 15; //phoneme actuel
    int next_etat = 15; //phoneme suivant
    std::vector<int> phrase;
    void set_neutre(){
        etat = 0;
        next_etat = 0;
    }
    void set_smile(){
        etat = 15;
        next_etat = 15;
    }
} Face;

typedef struct Camera {
    float viewx = 0.0f;
    float viewy = 0.0f;
    float viewz = 2.0f;
    //reset la position de la caméra 
    void reset() {
        viewx = 0.0f;
        viewy = 0.0f;
        viewz = 2.0f;
    }
} Camera;

extern Face facestruct;
extern Camera camera;   


//Structure pour manipuler points
typedef struct {
    double x;
    double y;
    double z;
    double w;
} Vec4;


//nombre de vertices (pas coordonnées, facteur 3)
extern int n;

//le set de vertices qui est réellement affiché
extern std::vector<float> face;
//Liste de toutes les phonèmes
extern const float neutre[];  
extern const float phoneme_A[2385];
extern const float phoneme_E[2385];
extern const float phoneme_I[2385];
extern const float phoneme_O[2385];
extern const float phoneme_U[2385];
extern const float phoneme_OU[2385];
extern const float phoneme_EH[2385];
extern const float phoneme_IN[2385];
extern const float phoneme_P[2385];
extern const float phoneme_K[2385];
extern const float phoneme_L[2385];
extern const float phoneme_W[2385];
extern const float phoneme_F[2385];
extern const float phoneme_S[2385];
extern const float smile[2385];
//Liste de toutes les connexions de vertices (formes les triangles)
extern const unsigned int indices[4260];


//Liste des phonèmes
extern const float* phonemes[16];

#endif