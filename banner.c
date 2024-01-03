#include "banner.h"
#include <stdio.h>

void show_banner(const char* file_path) {
    // Ouvrir le fichier en mode lecture
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Lire et afficher le contenu du fichier ligne par ligne
    char ligne[256];  // Taille plus grande pour une meilleure flexibilité
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        printf("%s", ligne);
    }

    // Fermer le fichier
    fclose(file);
}