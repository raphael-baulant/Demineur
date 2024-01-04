#ifndef BOARD_H
#define BOARD_H

#include "structures.h"

// Coordonnées du plateau de jeu
#define BOARD_COORDINATES_LENGTH 36
extern const char BOARD_COORDINATES[];

/*
 * Initialise le plateau de jeu en fonction de la difficulté spécifiée.
 * Paramètres : board - pointeur vers la structure du plateau à initialiser.
 *              difficulty - la difficulté du jeu.
 */
void init_board(Board *board, Difficulty difficulty);

/*
 * Alloue la mémoire nécessaire pour le plateau de jeu.
 * Paramètre : board - pointeur vers la structure du plateau à allouer.
 */
void alloc_board(Board *board);

/*
 * Libère la mémoire utilisée par le plateau de jeu.
 * Paramètre : board - pointeur vers la structure du plateau à libérer.
 */
void free_board(Board *board);

/*
 * Initialise les cellules du plateau de jeu.
 * Paramètre : board - pointeur vers la structure du plateau à initialiser.
 */
void init_cells(Board *board);

/*
 * Place les mines aléatoirement sur le plateau de jeu.
 * Paramètre : board - pointeur vers la structure du plateau où placer les mines.
 */
void place_mines(Board *board);

/*
 * Met à jour l'état du plateau de jeu en fonction du coup joué.
 * Paramètres : board - pointeur vers la structure du plateau à mettre à jour.
 *              move - le mouvement effectué par le joueur.
 */
void update_board(Board *board, Move move);

/*
 * Révèle récursivement les cellules voisines d'une position donnée.
 * Paramètres : board - pointeur vers la structure du plateau.
 *              position - la position dont les cellules voisines doivent être révélées.
 */
void reveal_neighbouring_cells_recursively(Board *board, Position position);

/*
 * Renvoie les positions des cellules voisines d'une position donnée sur le plateau.
 * Paramètres : board - la structure du plateau.
 *              position - la position pour laquelle obtenir les voisins.
 * Renvoie : la structure Neighbours contenant les positions des cellules voisines.
 */
Neighbours get_neighbours(Board board, Position position);

/*
 * Affiche le plateau de jeu.
 * Paramètres : board - la structure du plateau à afficher.
 *              show_mines - indique s'il faut afficher les mines.
 *              coloured - indique s'il faut utiliser la couleur pour l'affichage.
 */
void show_board(Board board, bool show_mines, bool coloured);

/*
 * Formate le nombre de mines adjacentes pour l'affichage.
 * Paramètres : adjacent_mines - le nombre de mines adjacentes.
 *              coloured - indique si l'affichage doit être coloré.
 * Renvoie : la chaîne de caractères formatée pour l'affichage.
 */
const char* format_adjacent_mines(int adjacent_mines, bool coloured);

#endif /* BOARD_H */