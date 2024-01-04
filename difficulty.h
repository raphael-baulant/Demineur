#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include "structures.h"

// Définition des constantes pour le choix de la difficulté
#define DIFFICULTY_EASY 1
#define DIFFICULTY_MEDIUM 2
#define DIFFICULTY_HARD 3

/*
 * Affiche les difficultés possibles pour le jeu.
 */
void show_difficulty();

/*
 * Sélectionne et retourne la difficulté choisie par l'utilisateur.
 * Renvoie : la difficulté sélectionnée.
 */
Difficulty select_difficulty();

/*
 * Vérifie si le choix de difficulté est valide en fonction de l'entrée utilisateur.
 * Paramètre : input - la chaîne de caractères représentant l'entrée utilisateur.
 * Renvoie : true si la difficulté est valide, sinon false.
 */
bool is_valid_difficulty_choice(const char *input);

#endif /* DIFFICULTY_H */