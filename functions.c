#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"

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
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].state == OUT) {
                printf(" ");
            }
            if (grid[i][j].state == HIDDEN) {
                if (!grid[i][j].mine) {
                    printf("Z");
                    //printf("%d", grid[i][j].adjacentMines);
                } else {
                    printf("M"); // Debbug
                }
            }
            if (grid[i][j].state == REVEALED) {
                if (!grid[i][j].mine) {
                    printf("%d", grid[i][j].adjacentMines);
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

Choice makeChoice(Cell** grid) {
    Position position;
    char action;
    bool isOut, isInvalidAction, isAlreadyRevealed, isAlreadyFlagged, isAlreadyUnflagged;
    do {
        printf("Choix : ");
        scanf("%d %d %c", &position.i, &position.j, &action);
        isOut = (position.i < 0 || position.j < 0 || position.i >= HEIGHT || position.j >= WIDTH) || (grid[position.i][position.j].state == OUT);
        isInvalidAction = (action != 'R' && action != 'S' && action != 'U');
        if (isOut || isInvalidAction) {
            if (isOut) {
                printf("Position choisie en dehors du X !\n");
            }
            if (isInvalidAction) {
                printf("Action choisie invalide !\n");
            }
        } else {
            isAlreadyRevealed = (grid[position.i][position.j].state == REVEALED);
            if (isAlreadyRevealed) {
                printf("Position choisie déjà révélée !\n");
            } else {
                bool isFlagged = (grid[position.i][position.j].state == FLAGGED);
                isAlreadyFlagged = (isFlagged && action == 'S');
                isAlreadyUnflagged = (!isFlagged && action == 'U');
                if (isAlreadyFlagged) {
                    printf("Position choisie déjà marquée !\n");
                }
                if (isAlreadyUnflagged) {
                    printf("Position choisie déjà non marquée !\n");
                }
            }
        }
    } while (isOut || isInvalidAction || isAlreadyRevealed || isAlreadyFlagged || isAlreadyUnflagged);
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