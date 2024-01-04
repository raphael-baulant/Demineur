#include "difficulty.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void show_difficulty() {
    printf("DIFFICULTY\n");
    printf("[%d] Easy\n", DIFFICULTY_EASY);
    printf("[%d] Medium\n", DIFFICULTY_MEDIUM);
    printf("[%d] Hard\n\n", DIFFICULTY_HARD);
}

Difficulty select_difficulty() {
    char input[100];
    int choice;

    while (1) {
        printf("\033[33m[Waiting]\033[0m Choice : ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (!is_valid_difficulty_choice(input)) {
            printf("\033[31m[Error]\033[0m Invalid choice.\n");
        } else {
            choice = atoi(input);
            break;
        }
    }

    switch (choice) {
        case DIFFICULTY_EASY:
            return EASY;
        case DIFFICULTY_MEDIUM:
            return MEDIUM;
        case DIFFICULTY_HARD:
            return HARD;
        default:
            return EASY;
    }
}

bool is_valid_difficulty_choice(const char *input) {
    char *endptr;
    long num = strtol(input, &endptr, 10);

    if (endptr == input || *endptr != '\0') {
        return false; // La chaîne n'est pas un nombre valide
    }

    if (num < DIFFICULTY_EASY || num > DIFFICULTY_HARD) {
        return false; // Le nombre est hors de la plage valide
    }

    return true; // La saisie est valide
}