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
Choice choose(Cell** grid);

// Retourne les positions des cellules voisines (dans la forme X)
Neighbours getNeighbours(Position position, Cell** grid);

#endif