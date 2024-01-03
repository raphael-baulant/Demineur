#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include "structures.h"

#define DIFFICULTY_EASY 1
#define DIFFICULTY_MEDIUM 2
#define DIFFICULTY_HARD 3

void show_difficulty();
int is_valid_difficulty_choice(const char *input);
Difficulty select_difficulty();

#endif /* DIFFICULTY_H */