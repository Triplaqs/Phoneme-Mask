#ifndef VERTEX_H
#define VERTEX_H


#include <vector>



//Structure pour manipuler les vertices du visage (abandonné)
typedef struct Point {
    float x, y, z;
    float w = 1.0f;
} Point;

//définition de singletons à manipuler en global
typedef struct Face {
    std::vector<float> points; //liste des vertices
    //int nb_pts;    // nombre de vertices (plus besoin, variable globale n)
    int etat = 0; //phoneme actuel
} Face;

typedef struct Camera {
    int viewx = 0;
    int viewy = 0;
    int viewz = 20;
    void reset() {
        viewx = 0;
        viewy = 0;
        viewz = 20;
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
extern const float neutre[2385];
extern const float Phoneme_A[2385];
//Liste de toutes les connexions de vertices (formes les triangles)
extern const unsigned int indices[4260];


//Liste des phonèmes
extern const float* phonemes[2];

#endif