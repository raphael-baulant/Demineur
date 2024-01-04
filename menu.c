#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void show_menu() {
    // Affiche les options du menu avec les choix associés
    printf("MENU\n");
    printf("[%d] Play\n", MENU_PLAY_GAME);
    printf("[%d] Leaderboard\n", MENU_LEADERBOARD);
    printf("[%d] Exit\n\n", MENU_QUIT);
}

int select_menu() {
    char input[100];
    int choice;

    while (1) {
        // Invite l'utilisateur à saisir son choix et récupère l'entrée utilisateur
        printf("\033[33m[Waiting]\033[0m Choice : ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Supprime le retour à la ligne s'il est présent

        // Vérifie si l'entrée est valide
        if (!is_valid_menu_choice(input)) {
            printf("\033[31m[Error]\033[0m Invalid choice.\n");
        } else {
            // Convertit l'entrée valide en entier et sort de la boucle
            choice = atoi(input);
            break;
        }
    }

    return choice;
}

bool is_valid_menu_choice(const char *input) {
    char *endptr;
    long num = strtol(input, &endptr, 10);

    // Vérifie si la conversion a réussi et si toute la chaîne a été convertie
    if (endptr == input || *endptr != '\0') {
        return false; // La chaîne n'est pas un nombre valide
    }

    // Vérifie si le nombre est dans la plage des choix du menu
    if (num < MENU_PLAY_GAME || num > MENU_QUIT) {
        return false; // Le nombre est hors de la plage valide
    }

    return true; // La saisie est valide
}