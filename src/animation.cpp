//OpenGL et rendering
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//Autres fichiers
#include "vertex.h"
#include "animation.h"
//autre
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

//constantes d'animations
float animStartTmps = -1.0f; //-1 si rien en cours
float animDuree = 0.1f; //durée d'une animation en secondes
float t = 0.0f; //temps pour interpolation

// Définition du dictionnaire d'association
std::unordered_map<std::string, int> dico_asso = {
    {" ", 0}, {". ", 0}, {" ;", 0}, {";", 0}, {"; ", 0}, {", ", 0}, {",", 0}, 
    {"A", 1}, {"a", 1}, {"À", 1}, {"à", 1}, {"Â", 1}, {"â", 1}, {"HA", 1}, {"hA", 1}, {"Ha", 1}, {"ha", 1}, {"HÀ", 1}, {"hÀ", 1}, {"Hà", 1}, {"hà", 1}, {"HÂ", 1}, {"hÂ", 1}, {"Hâ", 1}, {"hâ", 1}, 
    {"E", 2}, {"e", 2}, {"EU", 2}, {"eU", 2}, {"Eu", 2}, {"eu", 2},
    {"I", 3}, {"i", 3}, {"Î", 3}, {"î", 3}, {"HI", 3}, {"hI", 3}, {"Hi", 3}, {"hi", 3}, {"HÎ", 3}, {"hÎ", 3}, {"Hî", 3}, {"hî", 3},
    {"O", 4}, {"o", 4}, {"Ô", 4}, {"ô", 4}, {"HO", 4}, {"hO", 4}, {"Ho", 4}, {"ho", 4}, {"HÔ", 4}, {"hÔ", 4}, {"Hô", 4}, {"hô", 4},
    {"U", 5}, {"u", 5}, {"Û", 5}, {"û", 5},
    {"OU", 6}, {"oU", 6}, {"ou", 6}, {"Ou", 6}, {"OÙ", 6}, {"oÙ", 6}, {"où", 6}, {"Où", 6}, {"OÛ", 6}, {"oÛ", 6}, {"oû", 6}, {"Oû", 6}, 
    {"eh", 7}, {"è", 7}, {"ê", 7}, {"EH", 7}, {"È", 7}, {"Ê", 7}, {"Eh", 7},
    {"IN", 8}, {"iN", 8}, {"in", 8}, {"In", 8}, {"UN", 8}, {"uN", 8}, {"un", 8}, {"Un", 8}, {"EZ", 8}, {"eZ", 8}, {"ez", 8}, {"Ez", 8}, {"é", 8}, {"É", 8}, {"hé", 8}, {"hÉ", 8}, {"Hé", 8}, {"HÉ", 8},
    {"P", 9}, {"M", 9}, {"B", 9}, {"p", 9}, {"m", 9}, {"b", 9},
    {"C", 10}, {"D", 10}, {"G", 10}, {"K", 10}, {"R", 10}, {"T", 10}, {"TH", 10}, {"Th", 10}, {"Y", 10}, {"Z", 10}, {"J", 10}, {"H", 10},
    {"c", 10}, {"d", 10}, {"g", 10}, {"k", 10}, {"r", 10}, {"t", 10}, {"tH", 10}, {"th", 10}, {"y", 10}, {"z", 10}, {"j", 10}, {"h", 10},
    {"l", 11}, {"L", 11},
    {"W", 12}, {"Q", 12}, {"w", 12}, {"q", 12}, {"ON", 12}, {"oN", 12}, {"on", 12}, {"On", 12},
    {"f", 13}, {"v", 13}, {"F", 13}, {"V", 13}, 
    {"S", 14}, {"s", 14}, {"Ç", 14}, {"ç", 14}, {"N", 14}, {"n", 14},
    {"CH", 15}, {"cH", 15}, {" Ch", 15}, {"ch", 15},
    {" !", 16}, {"!", 16}, {" ?", 16}, {"?", 16}
};

//oi = O + A 


//Fonctions d'animation

//demande la prochaine séquence de caractères
void ask_string(){
    std::string input;
    std::cout << "Write a sentence !\n"; // Type a number and press enter
    //std::cin >> input; // Get user input from the keyboard
    std::getline(std::cin, input); //pour avoir les espaces avec
    //asup debug
    inputToPhrase(input);
}

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

//switched to phoneme (facestruc.next_etat) (edit quand la transition est finie)
void switchedTo(int i){
    //set up des variables globales d'animation
    //t = 0.0f; //re défini à chaque fois
    facestruct.etat = facestruct.next_etat;
    facestruct.next_etat = i;
}

//Transform the user string input into a list of phonetics attributed to facestruct.phrase
void inputToPhrase(std::string input){
    std::vector<int> phr;
    std::string text = input;
    size_t i = 0;
    while (i < text.size()) {
        bool ok = false;
        //on check en priorité 2 caractères
        if (i + 1 < text.size()) {
            std::string paire = text.substr(i, 2); //on regarde si les 2 prochains chars sont dans le dico asso
            if (dico_asso.count(paire)) {
                phr.push_back(dico_asso[paire]);
                i += 2; 
                ok = true;
            }
        }
        //on check 1 carac
        if (!ok) {
            std::string solo = text.substr(i, 1); // Extrait 1 char à partir de i
            if (dico_asso.count(solo)) {
                phr.push_back(dico_asso[solo]);
                i += 1; 
                ok = true;
            }
        }
        //caractère non reconnu par le dico -> erreur
        if (!ok) {
            std::cerr << "Erreur : Le caractère '" << text[i] << "' à l'index " << i << " est inconnu." << std::endl;
            return; //valid crashout
        }

    }
    facestruct.phrase = phr;
}

//update the phrase vector of facestruct and the current states and nextstate
void next_mouth(float currentTime){
    animStartTmps = currentTime; //on démarre l'anim
    if(facestruct.phrase.empty()){
        if(facestruct.next_etat!=16){
            facestruct.etat = facestruct.next_etat; //on établi le switch de phoneme
            facestruct.next_etat = 16; //on prépare la prochaine phoneme
        }
        else{
            facestruct.set_smile();
            ask_string();
        }
    }
    else{
        int phon = facestruct.phrase.front(); //on recup la prochaine phoneme
        facestruct.phrase.erase(facestruct.phrase.begin()); //on enlève le prochain element de la file d'attente
        facestruct.etat = facestruct.next_etat; //on établi le switch de phoneme
        facestruct.next_etat = phon; //on prépare la prochaine phoneme
    }
}

//affiche phrase pour débug
void display_phrase(){
    for(int i = 0; i<facestruct.phrase.size(); i++){
        printf("%d,", facestruct.phrase[i]);
    }
}
