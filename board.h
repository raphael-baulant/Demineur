#ifndef BOARD_H
#define BOARD_H

#include "structures.h"

#define BOARD_COORDINATES_LENGTH 36
extern const char BOARD_COORDINATES[];

void alloc_board(Board *board);
void free_board(Board *board);
void init_cells(Board *board);
Neighbours get_neighbours(Board board, Position position);
void place_mines(Board *board);
void init_board(Board *board, Difficulty difficulty);
void update_board(Board *board, Move move);
void reveal_empty_cells(Board *board, Position position);
void show_board(Board board, bool show_mines);
const char* get_colour(int adjacent_mines, bool colored);

#endif /* BOARD_H */