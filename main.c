#include <stdio.h>
#include <stdlib.h>
#include "banner.h"
#include "menu.h"
#include "difficulty.h"
#include "game.h"
#include "leaderboard.h"

int main() {
    while (1) {
        clear_screen();
        show_banner("banners/minesweeper.txt");
        show_menu();
        int choice = select_menu();
        clear_screen();
        switch (choice) {
            case MENU_PLAY_GAME:
                show_difficulty();
                Difficulty difficulty = select_difficulty();
                clear_screen();
                play_game(difficulty);
                printf("\033[33m[Waiting]\033[0m Press enter to return to the menu...");
                while (getchar() != '\n');
                break;
            case MENU_LEADERBOARD:
                show_banner("banners/leaderboard.txt");
                show_leaderboard(EASY);
                show_leaderboard(MEDIUM);
                show_leaderboard(HARD);
                printf("\033[33m[Waiting]\033[0m Press enter to return to the menu...");
                while (getchar() != '\n');
                break;
            case MENU_QUIT:
                show_banner("banners/minesweeper.txt");
                printf("Hope you enjoyed the game! Goodbye!\n");
                return 0;
            default:
                break;
        }
    }
}