#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"
#include <time.h>
#include <string.h>
#include <unistd.h>

int main() {
    system("clear"); // Efface le terminal

    if (HEIGHT < 3) {
        printf("[ERROR] The height parameter must be at least 2. Please correct it in constants.h file.");
        return 0;
    }

    // Affichage de la bannière + choix (Jouer/Scoreboard/Paramètres)?
    printTitle();
    getchar();
    system("clear");
    // Efface le terminal (pour "rafraichir")

    
    srand((unsigned int)time(NULL));
    Cell** grid = allocateGrid();
    initialiseGrid(grid);

    bool hasLost = false;
    bool hasWon = false;
    bool firstChoice = true;
    int mines = (int)(PROBA * Z + 0.5);
    int maxUnminedRevealedCells = Z - mines;
    int unminedRevealedCells = 0;
    int remainingMines = mines;

    time_t timeFirstChoice, timeLastChoice;
    int timer = 0; // seconds since the first action
    
    do {
        printf("[Reminder] A choice is a position (i,j) and an action C.\n");
        printf("          (C = (R)eveal, (S)et, (U)nset).\n\n");
        printf("[Info] Remaining mines : %d\n", remainingMines);
        print_timer(timer);
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

        if(firstChoice){
            timeFirstChoice = time(NULL);
            timeLastChoice = timeFirstChoice;
            firstChoice = false;
        }else{
            timeLastChoice = time(NULL);
            timer = timeLastChoice - timeFirstChoice;
        }
        system("clear");
        
    } while(!hasLost && !hasWon);

    if (hasWon) {
        printWin();
        print_timer(timer);
        // Ask the user to input his user name (record.txt);
    } else {
        printLose();
        print_timer(timer);
    }

    freeGrid(grid);
    return 0;
}