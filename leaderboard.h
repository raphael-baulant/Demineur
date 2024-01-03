#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "structures.h"

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

const char* get_file_path(Difficulty difficulty);
void update_leaderboard(char *player_name, Difficulty difficulty, Timer timer);
void show_leaderboard(Difficulty difficulty);

#endif /* LEADERBOARD_H */