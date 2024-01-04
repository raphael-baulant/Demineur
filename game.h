#ifndef GAME_H
#define GAME_H

#include "structures.h"

/*
 * Démarre une partie avec la difficulté spécifiée.
 * Paramètre : difficulty - la difficulté du jeu.
 */
void play_game(Difficulty difficulty);

/*
 * Affiche un rappel des commandes pour le jeu.
 */
void reminder();

/*
 * Affiche le nombre de mines restantes sur le plateau.
 * Paramètre : board - la structure du plateau de jeu.
 */
void show_remaining_mines(Board board);

/*
 * Sélectionne et retourne un mouvement saisi par le joueur.
 * Renvoie : le mouvement sélectionné par le joueur.
 */
Move select_move();

/*
 * Obtient la coordonnée à partir d'une entrée utilisateur.
 * Paramètre : input - la chaîne de caractères représentant l'entrée utilisateur.
 * Renvoie : la coordonnée obtenue à partir de l'entrée.
 */
int get_coordinate(char input);

/*
 * Vérifie si le mouvement sélectionné par le joueur est valide.
 * Paramètres : board - la structure du plateau de jeu.
 *              move - le mouvement effectué par le joueur.
 * Renvoie : true si le mouvement est valide, sinon false.
 */
bool check_valid_move(Board board, Move move);

/*
 * Vérifie si le joueur a gagné la partie.
 * Paramètre : board - la structure du plateau de jeu.
 * Renvoie : true si le joueur a gagné, sinon false.
 */
bool check_win(Board board);

/*
 * Vérifie si le joueur a perdu la partie.
 * Paramètre : board - la structure du plateau de jeu.
 * Renvoie : true si le joueur a perdu, sinon false.
 */
bool check_loss(Board board);

/*
 * Sélectionne et retourne le nom du joueur.
 * Renvoie : le nom du joueur.
 */
char* select_player_name();

/*
 * Archive la partie jouée (nom du joueur, difficulté, temps, état du plateau).
 * Paramètres : player_name - le nom du joueur.
 *              difficulty - la difficulté du jeu.
 *              timer - le temps passé à jouer.
 *              board - la structure du plateau de jeu.
 */
void archive_game(char *player_name, Difficulty difficulty, Timer timer, Board board);

/*
 * Obtient un timestamp pour l'archivage de la partie.
 * Renvoie : le timestamp au format chaîne de caractères.
 */
const char* get_timestamp();

#endif /* GAME_H */