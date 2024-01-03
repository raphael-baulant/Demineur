#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "structures.h"

const char* get_file_path(Difficulty difficulty);
void update_leaderboard(const char *player_name, Difficulty difficulty, Timer timer);
void show_leaderboard(Difficulty difficulty);

#endif /* LEADERBOARD_H */