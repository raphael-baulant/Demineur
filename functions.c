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

void displayGrid(Cell** grid) {
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
        printf("\n%02d \u2502", i);
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].state == OUT) {
                printf(" \u2591 \u2502");
            }
            if (grid[i][j].state == HIDDEN) {
                if (!grid[i][j].mine) {
                    printf("   \u2502"); // \u2588
                    //printf("%d", grid[i][j].adjacentMines);
                } else {
                    printf(" \u25A2 \u2502"); // Debbug \u25A0
                }
            }
            // dans les printf: affichage sur 3 de large avec la séparation à la fin
            if (grid[i][j].state == REVEALED) {
                if (!grid[i][j].mine) {
                    printf(" %d \u2502", grid[i][j].adjacentMines);
                } else {
                    printf(" M \u2502");
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
    do {
        printf("\n[+] choice (i j C) : ");
        int argcount = scanf("%d %d %c", &position.i, &position.j, &action);
        if(argcount == 3){
            isArg = false;
            break;
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

void printBanner(){
    const char *bannerFile = "banner.txt";

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
    printf("\n\n                          [+] Press \"Enter\" to play.\n");

    // Fermer le fichier
    fclose(fichier);
}

void printWin(){
    const char *bannerFile = "win.txt";

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
    printf("\n\n                   [+] You win in 00:00:00 (add time)\n");

    // Fermer le fichier
    fclose(fichier);
}

void printLose(){
    const char *bannerFile = "lose.txt";

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
    printf("\n");

    // Fermer le fichier
    fclose(fichier);      
}

void display_time() {
    time_t start_time;
    int seconds = 0;
    // Commencement NULL
    start_time = time(NULL);
    while(1) {
        seconds++;
        // affichage du temps écoulé 
        printf("\r%02d:%02d:%02d", seconds/3600, (seconds%3600)/60, seconds%60);
        //Permets le bon affichage de notre chrono
        fflush(stdout);
        // Attendre 1 seconde pour l'affichage c'est là que se joue le chrono
        sleep(1);
    }
}