#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

// Alloue l'emplacement mémoire nécessaire pour la grille du démineur
Cell** allocateGrid();

// Libère l'emplacement mémoire utilisé pour la grille du démineur
void freeGrid(Cell** grid);

// Initialise la grille du démineur
void initialiseGrid(Cell** grid);

// Affiche la grille actuelle du démineur
void displayGrid(Cell** grid);

// Retourne le choix du joueur
Choice makeChoice(Cell** grid);

// Exécute l'action du joueur
void executeAction(Choice choice, Cell** grid, int *unminedRevealedCells);

// Révèle en chaîne les cases s'il n'y a pas de mine voisine
void revealChain(Position position, Cell** grid, int *unminedRevealedCells);

// Retourne les positions des cellules voisines (dans la forme X)
Neighbours getNeighbours(Position position, Cell** grid);

// Affiche la bannière de début du jeu (Matheo)
void printBanner();

// Affiche le message de victoire (ajouter le temps?)
void printWin();

// Affiche le message de défaite
void printLose();

//Affiche le chronomètre
void print_timer();

#endif