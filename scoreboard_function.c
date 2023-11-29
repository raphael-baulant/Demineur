#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"
#include <time.h>
#include <string.h>
#include <unistd.h>
// file to avoid conflict in functions.c and .h (add theses functions later in functions.c and .h)
// ATTENTION A LA DIFFICULTEE !!!!

// function to get the 3 best score from scoreboard file

// function to print the podium (3 best score)
void printScoreboard(){
    const char *bannerFile = "banners/scoreboard.txt";

    // Ouvrir le fichier en mode lecture
    FILE *fichier = fopen(bannerFile, "r");

    // Vérifier si l'ouverture du fichier a réussi
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", bannerFile);
        exit;  // Quitter le programme avec un code d'erreur
    }

    // Lire et afficher le contenu du fichier ligne par ligne
    char ligne[100];  // Choisir une taille appropriée pour la ligne
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }
    printf("\n\n                          [+] Press \"Enter\" to play.\n");

    // Fermer le fichier
    fclose(fichier);
}

// 