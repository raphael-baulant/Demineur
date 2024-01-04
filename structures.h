#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

// État d'une cellule
typedef enum {
    OUT,
    HIDDEN,
    REVEALED,
    FLAGGED
} State;

// Structure représentant une cellule
typedef struct {
    bool is_mine;
    State state;
    int adjacent_mines;
} Cell;

// Structure représentant une position sur le plateau
typedef struct {
    int i;
    int j;
} Position;

// Structure représentant le plateau de jeu
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

// Structure représentant les positions des cellules voisines d'une cellule
typedef struct {
    int number;
    Position positions[8];
} Neighbours;

// Structure représentant un coup du joueur
typedef struct {
    Position position;
    char action;
} Move;

// Difficulté du jeu
typedef enum {
    EASY,
    MEDIUM,
    HARD
} Difficulty;

// Structure représentant un chronomètre pour mesurer le temps
typedef struct {
    int minutes;
    int seconds;
} Timer;

#endif /* STRUCTURES_H */