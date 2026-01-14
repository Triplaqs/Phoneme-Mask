#ifndef ANIMATION_H
#define ANIMATION_H


//Fonctions d'animation

//Prends un indice i et renvoie le phonème associé (voir attribution Annexe en dessous)
const float * getPhoneme(int i);

//switch to phoneme i
void switchTo(int i);

//Annexe :
/*
0  :  neutre[]
1 : phoneme_A[]
2 : phoneme_E[]
3 : phoneme_I[]
4 : phoneme_O[]
5 : phoneme_U[]
6 : phoneme_EU[]
7 : phoneme_OU[]
8 : phoneme_ANG[]
9 : phoneme_IN[]
10 : phoneme_ON[]
11 : phoneme_UN[]
12 : phoneme_AH[]
13 : phoneme_EH[]        
*/

//Anciennes fonctions d'affishage/edit fragment shader pour autres projets
void setTrianglePosition(unsigned int shaderProgram, float x, float y, float z = 0.0f, float w = 1.0f);

void setTriangleColor(unsigned int shaderProgram, float r, float g, float b, float a);

void generateFace();

void setTriangleColorRand(unsigned int shaderProgram);

void makeTriangleSpin(unsigned int shaderProgram, float time);



#endif