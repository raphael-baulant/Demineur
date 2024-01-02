#include "banner.h"

#include <stdio.h>

void show_banner(char* file_path) {
    // Ouvrir le fichier en mode lecture
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Lire et afficher le contenu du fichier ligne par ligne
    char ligne[100];  // Taille de la ligne
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        printf("%s", ligne);
    }

    // Fermer le fichier
    fclose(file);
}