#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

enum CellState {
    OUT,
    HIDDEN,
    REVEALED,
    FLAGGED
};

typedef struct {
    bool mine;
    enum CellState state;
    int adjacentMines;
} Cell;

typedef struct {
    int i;
    int j;
} Position;

typedef struct {
    Position position;
    char action;
} Choice;

typedef struct {
    int number;
    Position* positions;
} Neighbours;

typedef struct
{
    char username[50];
    int time;
} Score;

#endif