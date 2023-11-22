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
            if (grid[i][j].state == HIDDEN && !grid[i][j].mine) {
                printf("Z");
            }
            if (grid[i][j].state == REVEALED) {
                printf("1");
            }
            if (grid[i][j].state == FLAGGED) {
                printf("X");
            }
            if (grid[i][j].state == HIDDEN && grid[i][j].mine) {
                printf("M");
            }
        }
        printf("\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    int mines = (int)(PROBA * Z + 0.5);

    Cell** grid = allocateGrid();
    initialiseGrid(grid, mines);

    displayGrid(grid);

    //debbug avec print
    printf("%d", mines);

    freeGrid(grid);
    return 0;
}