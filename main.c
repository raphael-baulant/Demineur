#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"

int main() {
    system("clear"); // Efface le terminal

    if (HEIGHT < 3) {
        printf("[ERROR] The height parameter must be at least 2. Please correct it in constants.h file.");
        return 0;
    }

    // Affichage de la bannière + choix (Jouer/Scoreboard/Paramètres)?
    printBanner();
    getchar();
    system("clear"); // Efface le terminal (pour "rafraichir")
    
    srand((unsigned int)time(NULL));
    Cell** grid = allocateGrid();
    initialiseGrid(grid);

    bool hasLost = false;
    bool hasWon = false;
    int mines = (int)(PROBA * Z + 0.5);
    int maxUnminedRevealedCells = Z - mines;
    int unminedRevealedCells = 0;
    int remainingMines = mines;
    
    do {
        printf("A choice is a position (i,j) and an action C.\n(C = (R)eveal, (S)et, (U)nset).\n\n");
        printf("[+] Choice format : i j C\n\n");
        printf("[info] Remaining mines : %d\n\n", remainingMines);
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
        system("clear");
    } while(!hasLost && !hasWon);

    if (hasWon) {
        printWin();
    } else {
        printLose();
    }

    freeGrid(grid);
    return 0;
}