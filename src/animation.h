#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <cstdlib>
#include <unordered_map>


//constantes d'animations
extern float animStartTmps; //-1 si rien en cours
extern float animDuree; //durée d'une animation en secondes
extern float t; //temps pour interpolation
extern std::unordered_map<std::string, int> dico_asso; //associe les lettres à des numéros de phoneme
//Fonctions d'animation

//demande la prochaine séquence de caractères
void ask_string();

//Prends un indice i et renvoie le phonème associé (voir attribution Annexe en dessous)
const float * getPhoneme(int i);

//switch to phoneme i
void switchTo(int i, float currentTime);

//switched to phoneme i (edit quand la transition est finie)
void switchedTo(int i);

//Annexe :
/*
00 : neutre[]    (ponctuations de pauses)
01 : phoneme_A[]   (A, à, â)
02 : phoneme_E[]   (E)
03 : phoneme_I[]   (I, Î)
04 : phoneme_O[]   (O, Ô)
05 : phoneme_U[]   (U, Û)
06 : phoneme_OU[]  (OU, OÙ, OÛ)
07 : phoneme_EH[]  (EH, è, ê)
08 : phoneme_IN[]  (IN)
09 : phoneme_P[]   (P, M, B)
10 : phoneme_K[]   (C, D, G, K, R, T, Y, Z, J, N)
11 : phoneme_L[]   (L)
12 : phoneme_W[]   (W, Q)
13 : phoneme_F[]   (F, V)   
14 : phoneme_S[]   (S, ç)  
15 : phoneme_CH[]   (CH)    
16 : smile[]
*/

//Transform the user string input into a list of phonetics attributed to facestruct.phrase
void inputToPhrase(std::string input);

//update the phrase vector of facestruct and the current states and nextstate
void next_mouth(float currentTime);

//affiche phrase pour débug
void display_phrase();



#endif