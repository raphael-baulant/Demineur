#ifndef MENU_H
#define MENU_H

#define MENU_PLAY_GAME 1
#define MENU_LEADERBOARD 2
#define MENU_QUIT 3

void show_menu();
int is_valid_menu_choice(const char *input);
int select_menu();

#endif /* MENU_H */