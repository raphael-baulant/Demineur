#ifndef GAME_H
#define GAME_H

#include "structures.h"

void clear_screen();
bool check_win(Board board);
bool check_loss(Board board);
Move select_move();
bool check_valid_move(Board board, Move move);
void reminder();
void show_remaining_mines(Board board);
void play_game(Difficulty difficulty);
const char* select_player_name();
void archive_game(const char *player_name, Difficulty difficulty, Timer timer, Board board);
const char* get_timestamp();

#endif /* GAME_H */