#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

typedef enum {
    OUT,
    HIDDEN,
    REVEALED,
    FLAGGED
} State;

typedef struct {
    bool is_mine;
    State state;
    int adjacent_mines;
} Cell;

typedef struct {
    int i;
    int j;
} Position;

typedef struct {
    Cell **cells;
    int height;
    int width;
    int x_bar_width;
    int playable_cells;
    Position *playable_cells_positions;
    float mines_proba;
    int mines;
    int remaining_mines;
    int unmined_cells;
    int unmined_revealed_cells;
    bool loss;
} Board;

typedef struct {
    int number;
    Position positions[8];
} Neighbours;

typedef struct {
    Position position;
    char action;
} Move;

typedef enum {
    EASY,
    MEDIUM,
    HARD
} Difficulty;

typedef struct {
    int minutes;
    int seconds;
} Timer;

#endif /* STRUCTURES_H */