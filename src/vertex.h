#ifndef VERTEX_H
#define VERTEX_H


#include <vector>


//Structure pour manipuler les vertices du visage
typedef struct Point {
    float x, y, z;
    float w = 1.0f;
} Point;

typedef struct Face {
    std::vector<Point> points; //liste des vertices
    int nb_pts;    // nombre de vertices
    int etat = 0; //phoneme actuel
} Face;


//Structure pour manipuler points
typedef struct {
    double x;
    double y;
    double z;
    double w;
} Vec4;

// Vertices du triangle (global)
//extern float vertices[9];

extern float neutre[1248];
extern unsigned int indices[2136];


#endif