#include "difficulty.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void show_difficulty() {
    printf("DIFFICULTY\n");
    printf("[1] Easy\n");
    printf("[2] Medium\n");
    printf("[3] Hard\n\n");
}

Difficulty select_difficulty() {
    char input[100];
    int choice;

    while (1) {
        printf("\033[33m[Waiting]\033[0m Choice : ");
        fgets(input, sizeof(input), stdin);

        // Supprimer le saut de ligne de la fin de la chaîne
        input[strcspn(input, "\n")] = '\0';

        // Vérifier si la saisie contient uniquement des chiffres
        int valid_input = 1;
        for (size_t i = 0; i < strlen(input); i++) {
            if (!isdigit(input[i])) {
                valid_input = 0;
                break;
            }
        }

        // Convertir la chaîne en nombre si elle ne contient que des chiffres
        if (valid_input) {
            choice = atoi(input);

            // Vérifier si le nombre est entre 1 et 3
            if (choice < 1 || choice > 3) {
                printf("\033[31m[Error]\033[0m Invalid choice.\n");
            } else {
                break;
            }
        } else {
            printf("\033[31m[Error]\033[0m Invalid choice.\n");
        }
    }

    switch (choice) {
        case 1:
            return EASY;
        case 2:
            return MEDIUM;
        case 3:
            return HARD;
        default:
            return EASY;
    }
}