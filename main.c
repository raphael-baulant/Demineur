#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"

int main() {
    if (HEIGHT < 3) {
        printf("La hauteur doit être strictement supérieure à 2 !");
        return 0;
    }
    
    srand((unsigned int)time(NULL));
    Cell** grid = allocateGrid();
    initialiseGrid(grid);

    bool hasLost = false;
    bool hasWon = false;
    int mines = (int)(PROBA * Z + 0.5);
    int maxUnminedRevealedCells = Z - mines;
    int unminedRevealedCells = 0;
    int remainingMines = mines;
    printf("Un choix est une position (i,j) et une action C (C = (R)eveal, (S)et, (U)nset).\n");
    printf("Format du choix : i j C\n");
    do {
        printf("Mines : %d\n", remainingMines);
        displayGrid(grid);
        Choice choice = makeChoice(grid);
        executeAction(choice, grid, &unminedRevealedCells);
        if (choice.action == 'S') {
            remainingMines--;
        }
        if (choice.action == 'U') {
            remainingMines++;
        }
        if (choice.action == 'R' && grid[choice.position.i][choice.position.j].mine) {
            hasLost = true;
        }
        if (unminedRevealedCells == maxUnminedRevealedCells) {
            hasWon = true;
        }
    } while(!hasLost && !hasWon);

    if (hasWon) {
        printf("Vous avez gagné !");
    } else {
        printf("Vous avez perdu !");
    }

    freeGrid(grid);
    return 0;
}