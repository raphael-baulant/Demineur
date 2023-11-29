#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"
#include <time.h>
#include <string.h>
#include <unistd.h>

Cell** allocateGrid() {
    Cell** grid;
    grid = (Cell**)malloc(HEIGHT * sizeof(Cell*));
    for (int i = 0; i < HEIGHT; i++) {
        grid[i] = (Cell*)malloc(WIDTH * sizeof(Cell));
    }
    return grid;
}

void freeGrid(Cell** grid) {
    for (int i = 0; i < HEIGHT; i++) {
        free(grid[i]);
    }
    free(grid);
}

void initialiseGrid(Cell** grid) {
    Position minesPossiblePositions[Z];
    int n = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (((i <= HEIGHT/2) && ((j >= i && j < i+X) || (j >= WIDTH-(i+X) && j < WIDTH-i))) || ((i >= HEIGHT/2) && ((j >= HEIGHT-i-1 && j < HEIGHT-i-1+X) || (j >= WIDTH-(HEIGHT-i-1+X)) && j < WIDTH-(HEIGHT-i-1)))) {
                grid[i][j].state = HIDDEN;
                grid[i][j].mine = false;
                grid[i][j].adjacentMines = 0;
                minesPossiblePositions[n].i = i;
                minesPossiblePositions[n].j = j;
                n++;
            } else {
                grid[i][j].state = OUT;
            }
        }
    }
    // Placement des mines
    int mines = (int)(PROBA * Z + 0.5);
    int count = 0;
    while (count < mines) {
        int random = rand() % Z;
        Position position = minesPossiblePositions[random];
        if (!grid[position.i][position.j].mine) {
            grid[position.i][position.j].mine = true;
            count++;
            // Gestion des mines adjacentes
            Neighbours neighbours = getNeighbours(position, grid);
            for (int k = 0; k < neighbours.number; k++) {
                grid[neighbours.positions[k].i][neighbours.positions[k].j].adjacentMines++;
            }
            free(neighbours.positions);
        }
    }
}

Neighbours getNeighbours(Position position, Cell** grid) {
    Neighbours neighbours;
    neighbours.number = 0;
    neighbours.positions = malloc(8 * sizeof(Position));
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i != 0 || j != 0) {
                int k = position.i + i;
                int l = position.j + j;
                if (k >= 0 && k < HEIGHT && l >= 0 && l < WIDTH) {
                    if (grid[position.i][position.j].state != OUT) {
                        Position position = {k, l};
                        neighbours.positions[neighbours.number] = position;
                        neighbours.number++;
                    }
                }
            }
        }
    }
    return neighbours;
}

void displayGrid(Cell** grid, bool hasLost) {
    // affichage des coordonnées en haut :
    printf("\u267F \u2502");
    for (int j = 0; j < WIDTH; j++){
        printf("%02d \u2502", j);
    }
    for (int i = 0; i < HEIGHT; i++) {
        printf("\n");
        //séparation de chaque ligne
        for (int j = 0; j <= WIDTH; j++) {
            printf("\u2500\u2500\u2500\u253C");
        }
        printf(" \n%02d \u2502", i);
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].state == OUT) {
                printf(" \u2592 \u2502");
            }
            if (grid[i][j].state == HIDDEN) {
                if (!grid[i][j].mine) {
                    printf("   \u2502"); // \u2588
                    //printf("%d", grid[i][j].adjacentMines);
                } else {
                    if(hasLost){
                        printf(" \u25A0 \u2502"); //pour l'affichage défaite
                    }
                    else{
                        printf(" \u26A0 \u2502"); // Debbug \u25A0
                    }
                }
            }
            // dans les printf: affichage sur 3 de large avec la séparation à la fin
            if (grid[i][j].state == REVEALED) {
                if (!grid[i][j].mine) {
                    printf(" %d \u2502", grid[i][j].adjacentMines);
                } else {
                    printf(" \u25A2 \u2502");
                }
            }
            if (grid[i][j].state == FLAGGED) {
                printf(" \u2691 \u2502"); // \u2691
            }
        }
    }
    printf("\n");
    for (int j = 0; j <= WIDTH; j++) {
            printf("\u2500\u2500\u2500\u2534");
    }
    printf("\n");
}

Choice makeChoice(Cell** grid) {
    Position position;
    char action;
    bool isArg, isOut, isInvalidAction, isAlreadyRevealed, isAlreadyFlagged, isAlreadyUnflagged;
    char input[100];
    do {
        printf("\n[+] choice (i j C) : ");
        fgets(input, sizeof(input), stdin);
        int argcount = sscanf(input, "%d %d %c", &position.i, &position.j, &action);
        if(argcount != 3){
            isArg = false;
            printf("\n[Error] Invalid input.\n");
            continue;
        }
        isArg = true;
        isOut = (position.i < 0 || position.j < 0 || position.i >= HEIGHT || position.j >= WIDTH) || (grid[position.i][position.j].state == OUT);
        isInvalidAction = (action != 'R' && action != 'S' && action != 'U');
        if (isOut || isInvalidAction) {
            if (isOut) {
                printf("\n[Error] position is outside of the gameboard (X).\n");
            }
            if (isInvalidAction) {
                printf("\n[Error] Invalid action.\n");
            }
        } else {
            isAlreadyRevealed = (grid[position.i][position.j].state == REVEALED);
            if (isAlreadyRevealed) {
                printf("\n[Error] position already revealed.\n");
            } else {
                bool isFlagged = (grid[position.i][position.j].state == FLAGGED);
                isAlreadyFlagged = (isFlagged && action == 'S');
                isAlreadyUnflagged = (!isFlagged && action == 'U');
                if (isAlreadyFlagged) {
                    printf("\n[Error] position already set (flagged).\n");
                }
                if (isAlreadyUnflagged) {
                    printf("\n[Error] position already unset (unflagged).\n");
                }
            }
        }
    } while (!isArg || isOut || isInvalidAction || isAlreadyRevealed || isAlreadyFlagged || isAlreadyUnflagged);
    Choice choice = {position , action};
    return choice;
}

void executeAction(Choice choice, Cell** grid, int *unminedRevealedCells) {
    if (choice.action == 'S') {
        grid[choice.position.i][choice.position.j].state = FLAGGED;
    }
    if (choice.action == 'U') {
        grid[choice.position.i][choice.position.j].state = HIDDEN;
    }
    if (choice.action == 'R') {
        grid[choice.position.i][choice.position.j].state = REVEALED;
        bool isMine = grid[choice.position.i][choice.position.j].mine;
        if (!isMine) {
            (*unminedRevealedCells)++;
            if (grid[choice.position.i][choice.position.j].adjacentMines == 0) {
                revealChain(choice.position, grid, unminedRevealedCells);
            }
        }
    }
}

void revealChain(Position position, Cell** grid, int *unminedRevealedCells) {
    Neighbours neighbours = getNeighbours(position, grid);
    for (int k = 0; k < neighbours.number; k++) {
        if (grid[neighbours.positions[k].i][neighbours.positions[k].j].state == HIDDEN) {
            grid[neighbours.positions[k].i][neighbours.positions[k].j].state = REVEALED;
            (*unminedRevealedCells)++;
            if (grid[neighbours.positions[k].i][neighbours.positions[k].j].adjacentMines == 0) {
                revealChain(neighbours.positions[k], grid, unminedRevealedCells);
            }
        }
    }
    free(neighbours.positions);
}

void print_timer(int x) {
    int h,m,s;
    h = x / 3600;
    m = (x % 3600) / 60;
    s = (x % 3600) % 60;
    printf("[Info] Timer : %02d:%02d:%02d\n\n", h,m,s);
}

void printBanner(char* bannerFile){
    // Ouvrir le fichier en mode lecture
    FILE *fichier = fopen(bannerFile, "r");

    // Vérifier si l'ouverture du fichier a réussi
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", bannerFile);
        exit;  // Quitter le programme avec un code d'erreur
    }

    // Lire et afficher le contenu du fichier ligne par ligne
    char ligne[100];  // Choisir une taille appropriée pour la ligne
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }
    // Fermer le fichier
    fclose(fichier);
}

void printMenu(){
    printBanner("banners/menu.txt");
    printf("\n\n                          [0] Play\n");
    printf("                          [1] Leaderboard\n");
    printf("                          [2] Exit\n\n");
}   

void printLeaderboard(){
    printBanner("banners/leaderboard.txt");
    printf("\n[Info] Difficulty : %d\n\n",HEIGHT);
    // Ajouter l'affichage des 3 meilleurs joueurs (fonction printLeader() => fonction getLeader())
    printf("[+] Press \"Enter\" for the menu\n");
    sleep(3); // A SUPPRIMER 
}

void printWin(){
    printBanner("banners/win.txt");
}

void printLose(){
    printBanner("banners/lose.txt");
}

void addRecord(char* username, int timer){
    const char *recordFile = "record.txt";

    // Ouvrir le fichier en mode lecture
    FILE *file = fopen(recordFile, "a");

    if (file == NULL) {
        fprintf(stderr, "Can't open file %s\n", recordFile);
        exit;  // Quitter le programme avec un code d'erreur
    }

    fprintf(file, "%d:%s:%d\n", HEIGHT, username, timer);  // Écriture dans le fichier

    fclose(file);  // Fermeture du fichier
}

