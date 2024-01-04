#include "difficulty.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void show_difficulty() {
    // Affiche les options de difficulté avec les choix associés
    printf("DIFFICULTY\n");
    printf("[%d] Easy\n", DIFFICULTY_EASY);
    printf("[%d] Medium\n", DIFFICULTY_MEDIUM);
    printf("[%d] Hard\n\n", DIFFICULTY_HARD);
}

Difficulty select_difficulty() {
    char input[100];
    int choice;

    while (1) {
        // Invite l'utilisateur à saisir son choix et récupère l'entrée utilisateur
        printf("\033[33m[Waiting]\033[0m Choice : ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Supprime le retour à la ligne s'il est présent

        // Vérifie si l'entrée est valide
        if (!is_valid_difficulty_choice(input)) {
            printf("\033[31m[Error]\033[0m Invalid choice.\n");
        } else {
            // Convertit l'entrée valide en entier et sort de la boucle
            choice = atoi(input);
            break;
        }
    }

    // Renvoie le niveau de difficulté correspondant au choix de l'utilisateur
    switch (choice) {
        case DIFFICULTY_EASY:
            return EASY;
        case DIFFICULTY_MEDIUM:
            return MEDIUM;
        case DIFFICULTY_HARD:
            return HARD;
        default:
            return EASY; // Si le choix n'est pas valide, retourne la difficulté facile par défaut
    }
}

bool is_valid_difficulty_choice(const char *input) {
    char *endptr;
    long num = strtol(input, &endptr, 10);

    // Vérifie si la conversion a réussi et si toute la chaîne a été convertie
    if (endptr == input || *endptr != '\0') {
        return false; // La chaîne n'est pas un nombre valide
    }

    // Vérifie si le nombre est dans la plage des choix de difficulté
    if (num < DIFFICULTY_EASY || num > DIFFICULTY_HARD) {
        return false; // Le nombre est hors de la plage valide
    }

    return true; // La saisie est valide
}