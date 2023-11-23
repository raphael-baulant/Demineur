#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "constants.h"

enum CellState {
    OUT,
    HIDDEN,
    REVEALED,
    FLAGGED
};

typedef struct {
    bool mine;
    enum CellState state;
    int adjacentMineCount;

} Cell;

typedef struct {
    int i;
    int j;
} Position;

typedef struct {
    Position position;
    char action;
} Choice;

Cell** allocateGrid() {
    Cell** grid;
    grid = (Cell**)malloc(HEIGHT * sizeof(Cell*));
    if (grid == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les lignes de la matrice.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HEIGHT; i++) {
        grid[i] = (Cell*)malloc(WIDTH * sizeof(Cell));
        if (grid[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour les colonnes de la matrice.\n");
            exit(EXIT_FAILURE);
        }
    }

    return grid;
}

void freeGrid(Cell** grid) {
    for (int i = 0; i < HEIGHT; i++) {
        free(grid[i]);
    }
    free(grid);
}

void initialiseGrid(Cell** grid, int mines) {
    Position minesPossiblePositions[Z];
    int n = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (((i <= HEIGHT/2) && ((j >= i && j < i+X) || (j >= WIDTH-(i+X) && j < WIDTH-i))) || ((i >= HEIGHT/2) && ((j >= HEIGHT-i-1 && j < HEIGHT-i-1+X) || (j >= WIDTH-(HEIGHT-i-1+X)) && j < WIDTH-(HEIGHT-i-1)))) {
                grid[i][j].mine = false;
                grid[i][j].state = HIDDEN;
                grid[i][j].adjacentMineCount = 0;
                minesPossiblePositions[n].i = i;
                minesPossiblePositions[n].j = j;
                n++;
            } else {
                grid[i][j].state = OUT;
            }
        }
    }
    int count = 0;
    while (count < mines) {
        int random = rand() % Z;
        Position position = minesPossiblePositions[random];
        if (!grid[position.i][position.j].mine) {
            grid[position.i][position.j].mine = true;
            count++;
        }
    }
}

void displayGrid(Cell** grid) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].state == OUT) {
                printf(" ");
            }
            if (grid[i][j].state == HIDDEN) {
                if (!grid[i][j].mine) {
                    printf("Z");
                } else {
                    printf("M");
                }
            }
            if (grid[i][j].state == REVEALED) {
                if (!grid[i][j].mine) {
                    printf("1");
                } else {
                    printf("M");
                }
            }
            if (grid[i][j].state == FLAGGED) {
                printf("F");
            }
        }
        printf("\n");
    }
}

Choice choose(Cell** grid) {
    Position position;
    char action;
    bool out, invalidAction;
    printf("Entrez une position (i,j) et une action C (C = (R)eveal, (S)et, (U)nset).\n");
    printf("Format du choix : i j C\n");
    do {
        printf("Choix : ");
        scanf("%d %d %c", &position.i, &position.j, &action);
        out = (position.i < 0 || position.j < 0 || position.i >= HEIGHT || position.j >= WIDTH) || (grid[position.i][position.j].state == OUT);
        if (out) {
            printf("Position choisie invalide !\n");
        }
        invalidAction = (action != 'R' && action != 'S' && action != 'U');
        if (invalidAction) {
            printf("Action choisie invalide !\n");
        }
    } while (out || invalidAction);
    Choice choice = {position , action};
    return choice;
}

int main() {
    srand((unsigned int)time(NULL));
    int mines = (int)(PROBA * Z + 0.5);

    Cell** grid = allocateGrid();
    initialiseGrid(grid, mines);
    displayGrid(grid);

    Choice choice = choose(grid);

    //debbug avec print
    printf("Choix : %d %d %c\n", choice.position.i, choice.position.j, choice.action);
    printf("Mines : %d\n", mines);

    freeGrid(grid);
    return 0;
}