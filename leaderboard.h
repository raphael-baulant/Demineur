#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "structures.h"

void update_leaderboard(const char *player_name, Difficulty difficulty, Timer timer);
void show_leaderboard(Difficulty difficulty);
//void load_leaderboard();

#endif /* LEADERBOARD_H */