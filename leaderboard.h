#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "structures.h"

// Définition des constantes pour le leaderboard
#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

/*
 * Obtient le chemin du fichier pour le classement en fonction de la difficulté.
 * Paramètre : difficulty - la difficulté pour laquelle on souhaite obtenir le chemin du fichier.
 * Renvoie : le chemin du fichier correspondant à la difficulté spécifiée.
 */
const char* get_file_path(Difficulty difficulty);

/*
 * Met à jour le classement avec le nom du joueur et son temps pour une difficulté donnée.
 * Paramètres : player_name - le nom du joueur.
 *              difficulty - la difficulté du jeu.
 *              timer - le temps pris pour terminer le jeu.
 */
void update_leaderboard(char *player_name, Difficulty difficulty, Timer timer);

/*
 * Affiche le classement pour une certaine difficulté.
 * Paramètre : difficulty - la difficulté pour laquelle afficher le classement.
 */
void show_leaderboard(Difficulty difficulty);

#endif /* LEADERBOARD_H */