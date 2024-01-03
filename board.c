#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char BOARD_COORDINATES[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

#define EASY_HEIGHT 5
#define MEDIUM_HEIGHT 9
#define HARD_HEIGHT 13

#define EASY_MINES_PROBA 0.1
#define MEDIUM_MINES_PROBA 0.15
#define HARD_MINES_PROBA 0.2

void alloc_board(Board *board) {
    board->cells = (Cell**)malloc(board->height * sizeof(Cell*));
    for (int i = 0; i < board->height; i++) {
        board->cells[i] = (Cell*)malloc(board->width * sizeof(Cell));
    }
    board->playable_cells_positions = (Position*)malloc(board->playable_cells * sizeof(Position));
}

void free_board(Board *board) {
    for (int i = 0; i < board->height; i++) {
        free(board->cells[i]);
    }
    free(board->cells);
    free(board->playable_cells_positions);
}

void init_cells(Board *board) {
    int n = 0;
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            bool x_top = (i <= board->height / 2);
            bool x_top_left = (j >= i) && (j < i + board->x_bar_width);
            bool x_top_right = (j >= board->width - (i + board->x_bar_width)) && (j < board->width - i);
            bool x_bottom = (i >= board->height / 2);
            bool x_bottom_left = (j >= board->height - i - 1) && (j < board->height - i - 1 + board->x_bar_width);
            bool x_bottom_right = (j >= board->width - (board->height - i - 1 + board->x_bar_width)) && (j < board->width - (board->height - i - 1));
            
            bool is_playable_cell = (x_top && (x_top_left || x_top_right)) || (x_bottom && (x_bottom_left || x_bottom_right));
            if (is_playable_cell) {
                board->cells[i][j].state = HIDDEN;
                board->cells[i][j].is_mine = false;
                board->cells[i][j].adjacent_mines = 0;
                
                board->playable_cells_positions[n].i = i;
                board->playable_cells_positions[n].j = j;
                n++;
            } else {
                board->cells[i][j].state = OUT;
            }
        }
    }
}

Neighbours get_neighbours(Board board, Position position) {
    Neighbours neighbours;
    neighbours.number = 0;
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i != 0 || j != 0) {
                int k = position.i + i;
                int l = position.j + j;
                
                if (k >= 0 && k < board.height && l >= 0 && l < board.width) {
                    if (board.cells[position.i][position.j].state != OUT) {
                        Position neighbour_position = {k, l};
                        neighbours.positions[neighbours.number] = neighbour_position;
                        neighbours.number++;
                    }
                }
            }
        }
    }
    
    return neighbours;
}

void place_mines(Board *board) {
    int count = 0;
    while (count < board->mines) {
        int random = rand() % board->playable_cells;
        Position position = board->playable_cells_positions[random];
        if (!board->cells[position.i][position.j].is_mine) {
            board->cells[position.i][position.j].is_mine = true;
            count++;
            // Gestion des mines adjacentes
            Neighbours neighbours = get_neighbours(*board, position);
            for (int k = 0; k < neighbours.number; k++) {
                board->cells[neighbours.positions[k].i][neighbours.positions[k].j].adjacent_mines++;
            }
        }
    }
}

void init_board(Board *board, Difficulty difficulty) {
    switch (difficulty) {
        case EASY:
            board->height = EASY_HEIGHT;
            board->mines_proba = EASY_MINES_PROBA;
            break;
        case MEDIUM:
            board->height = MEDIUM_HEIGHT;
            board->mines_proba = MEDIUM_MINES_PROBA;
            break;
        case HARD:
            board->height = HARD_HEIGHT;
            board->mines_proba = HARD_MINES_PROBA;
            break;
        default:
            board->height = EASY_HEIGHT;
            board->mines_proba = EASY_MINES_PROBA;
            break;
    }
    if (board->height % 2) {
        board->x_bar_width = board->height - 1;
    } else {
        board->x_bar_width = board->height - 2;
    }
    board->width = 3 * board->x_bar_width;
    board->playable_cells = 2 * board->x_bar_width * board->height;
    board->mines = (int)(board->mines_proba * board->playable_cells + 0.5);
    board->remaining_mines = board->mines;
    board->unmined_cells = board->playable_cells - board->mines;
    board->unmined_revealed_cells = 0;
    board->loss = false;

    alloc_board(board);
    init_cells(board);
    place_mines(board);
}

void update_board(Board *board, Move move) {
    Position position = move.position;
    switch (move.action) {
        case 'S':
            board->cells[position.i][position.j].state = FLAGGED;
            board->remaining_mines--;
            break;
        case 'U':
            board->cells[position.i][position.j].state = HIDDEN;
            board->remaining_mines++;
            break;
        case 'R':
            board->cells[position.i][position.j].state = REVEALED;
            if (!board->cells[position.i][position.j].is_mine) {
                board->unmined_revealed_cells++;
                reveal_empty_cells(board, position);
            } else {
                board->loss = true;
            }
            break;
        default:
            break;
    }
}

void reveal_empty_cells(Board *board, Position position) {
    Neighbours neighbours = get_neighbours(*board, position);
    int adjacent_mines = board->cells[position.i][position.j].adjacent_mines;
    if (adjacent_mines == 0) {
        for (int k = 0; k < neighbours.number; k++) {
            if (board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state == HIDDEN) {
                board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state = REVEALED;
                board->unmined_revealed_cells++;
                reveal_empty_cells(board, neighbours.positions[k]);
            }
        }
    } else {
        int flagged_neighbours = 0;
        for (int k = 0; k < neighbours.number; k++) {
            if (board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state == FLAGGED) {
                flagged_neighbours++;
            }
        }
        if (flagged_neighbours == adjacent_mines) {
            for (int k = 0; k < neighbours.number; k++) {
                if (board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state == HIDDEN) {
                    board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state = REVEALED;
                    if (!board->cells[neighbours.positions[k].i][neighbours.positions[k].j].is_mine) {
                        board->unmined_revealed_cells++;
                        reveal_empty_cells(board, neighbours.positions[k]);
                    } else {
                        board->loss = true;
                    }
                }
            }
        }
    }
}

void show_board(Board board, bool show_mines) {
    printf("   ");
    for (int j = 0; j < board.width; j++) {
        printf("  %c ", BOARD_COORDINATES[j]);
    }
    printf("\n");
    
    for (int i = 0; i < board.height; i++) {
        printf("   ");
        if (i == 0) {
            for (int j = 0; j < board.width; j++) {
                if (j == 0) {
                    printf("\u250C\u2500\u2500\u2500");
                } else {
                    printf("\u252C\u2500\u2500\u2500");
                }
            }
            printf("\u2510 \n");
        } else {
            for (int j = 0; j < board.width; j++) {
                if (j == 0) {
                    printf("\u251C\u2500\u2500\u2500");
                } else {
                    printf("\u253C\u2500\u2500\u2500");
                }
            }
            printf("\u2524 \n");
        }
        
        printf(" %c ", BOARD_COORDINATES[i]);
        for (int j = 0; j < board.width; j++) {
            Cell cell = board.cells[i][j];
            if (cell.state == OUT) {
                printf("\u2502 \u2592 ");
            }
            if (cell.state == FLAGGED) {
                if (!show_mines || cell.is_mine) {
                    printf("\u2502 \u2691 ");
                } else {
                    printf("\u2502 X ");
                }
            }
            if (cell.state == HIDDEN) {
                if (cell.is_mine) {
                    printf("\u2502 \u25A0 ");
                } else if (!show_mines || !cell.is_mine) {
                    printf("\u2502   ");
                } else {
                    printf("\u2502 \u25A0 ");
                }
            }
            if (cell.state == REVEALED) {
                if (!show_mines || !cell.is_mine) {
                    printf("\u2502 %s ", get_colour(cell.adjacent_mines, true));
                } else {
                    printf("\u2502 \u25A2 ");
                }
            }
        }
        printf("\u2502 %c \n", BOARD_COORDINATES[i]);
    }

    printf("   ");
    for (int j = 0; j < board.width; j++) {
        if (j == 0) {
            printf("\u2514\u2500\u2500\u2500");
        } else {
            printf("\u2534\u2500\u2500\u2500");
        }
    }
    printf("\u2518 \n");
    
    printf("   ");
    for (int j = 0; j < board.width; j++) {
        printf("  %c ", BOARD_COORDINATES[j]);
    }
    printf("\n\n");
}

const char* get_colour(int adjacent_mines, bool colored) {
    static char output[20];

    if (adjacent_mines == 0) {
        return ".";
    }
    if (colored) {
        if (adjacent_mines == 1) {
            sprintf(output, "\033[34m%d\033[0m", adjacent_mines);
        } else if (adjacent_mines == 2) {
            sprintf(output, "\033[32m%d\033[0m", adjacent_mines);
        } else if (adjacent_mines == 3) {
            sprintf(output, "\033[33m%d\033[0m", adjacent_mines);
        } else {
            sprintf(output, "\033[31m%d\033[0m", adjacent_mines);
        }
    } else {
        sprintf(output, "%d", adjacent_mines);
    }

    return output;
}