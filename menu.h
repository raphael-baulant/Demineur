#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

// Définition des constantes pour le choix du menu
#define MENU_PLAY_GAME 1
#define MENU_LEADERBOARD 2
#define MENU_QUIT 3

/*
 * Affiche le menu du jeu.
 */
void show_menu();

/*
 * Sélectionne et retourne le choix du menu choisi par l'utilisateur.
 * Renvoie : le choix du menu sélectionné.
 */
int select_menu();

/*
 * Vérifie si le choix du menu est valide en fonction de l'entrée utilisateur.
 * Paramètre : input - la chaîne de caractères représentant l'entrée utilisateur.
 * Renvoie : true si le choix est valide, sinon false.
 */
bool is_valid_menu_choice(const char *input);

#endif /* MENU_H */