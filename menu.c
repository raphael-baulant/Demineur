#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void show_menu() {
    printf("MENU\n");
    printf("[%d] Play\n", MENU_PLAY_GAME);
    printf("[%d] Leaderboard\n", MENU_LEADERBOARD);
    printf("[%d] Exit\n\n", MENU_QUIT);
}

int is_valid_menu_choice(const char *input) {
    char *endptr;
    long num = strtol(input, &endptr, 10);

    if (endptr == input || *endptr != '\0') {
        return 0; // La chaîne n'est pas un nombre valide
    }

    if (num < MENU_PLAY_GAME || num > MENU_QUIT) {
        return 0; // Le nombre est hors de la plage valide
    }

    return 1; // La saisie est valide
}

int select_menu() {
    char input[100];
    int choice;

    while (1) {
        printf("\033[33m[Waiting]\033[0m Choice : ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (!is_valid_menu_choice(input)) {
            printf("\033[31m[Error]\033[0m Invalid choice.\n");
        } else {
            choice = atoi(input);
            break;
        }
    }

    return choice;
}