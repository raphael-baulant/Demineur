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

    int menuChoice; // Choix de l'utilisateur (0=Play, 1=Leaderboard, 2=Exit)
    bool exit = false; // Permet de rester dans le menu tant que l'utilisateur n'a pas choisi de sortir

    while(!exit){ 
        printMenu();
        char input[100];
        fgets(input, sizeof(input), stdin);
        int argcount = sscanf(input, "%d", &menuChoice);
        if(argcount != 1) {
            printf("\n[Error] Invalid choice.\n");
            continue;
        }

        /*
            MENU CHOICE 0 --> PLAY GAME
        */
        if(menuChoice == 0){

            // initialize game (create a struct and initialization function!)
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

            // play loop
            do {
                system("clear");
                printf("[Reminder] A choice is a position (i,j) and an action C.\n");
                printf("          (C = (R)eveal, (S)et, (U)nset).\n\n");
                printf("[Info] Remaining mines : %d\n", remainingMines);
                print_timer(timer);
                displayGrid(grid,false);
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
<<<<<<< HEAD
                hasWon = true; //debbug
=======
                hasWon = true;
>>>>>>> 6a3a31a2bb1e08e3afe323386acd13940e939570
            } while(!hasLost && !hasWon);

            // game end (win or lose)
            if (hasWon) {
                printWin();
                print_timer(timer);
<<<<<<< HEAD
                // Ask the user to input his user name (record.txt);
                saveWinningGame("timer");
=======
                printf("\nPlease enter your username : ");
                char username[50];
                scanf("%s",&username);
                addRecord(username, timer);
>>>>>>> 6a3a31a2bb1e08e3afe323386acd13940e939570
            } else {
                displayGrid(grid, true);
                printf("\n\n");
                printLose();
                printf("\n\n"); //fix chrono display
                print_timer(timer);
                sleep(5); // Faire en sorte que le joueur doive presser entrée pour revenir au menu
            }
            freeGrid(grid);
        }

        /*
            MENU CHOICE = 1 --> LEADERBOARD
        */
        if(menuChoice == 1){
            system("clear");
            printLeaderboard();
            getchar(); // fonctionne pas
        } 

        /*
            MENU CHOICE = 2 --> EXIT
        */
        if(menuChoice == 2){
            exit = true;
        }
        system("clear"); // Efface le terminal (pour "rafraichir")
    }
    return 0; 
}