#include "banner.h"
#include <stdio.h>

void show_banner(const char* file_path) {
    // Ouvre le fichier en mode lecture
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        // Affiche un message d'erreur si le fichier n'est pas ouvert avec succès
        perror("Error opening file");
        return;
    }

    // Lit et affiche le contenu du fichier ligne par ligne
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        printf("%s", ligne);
    }

    // Ferme le fichier
    fclose(file);
}