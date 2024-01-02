#include "leaderboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

char* get_file_path(Difficulty difficulty) {
    char* file_path;

    switch (difficulty) {
        case EASY:
            file_path = "leaderboard/easy.txt";
            break;
        case MEDIUM:
            file_path = "leaderboard/medium.txt";
            break;
        case HARD:
            file_path = "leaderboard/hard.txt";
            break;
        default:
            file_path = "leaderboard/easy.txt";
            break;
    }

    return file_path;
}

void update_leaderboard(const char *player_name, Difficulty difficulty, Timer timer) {
    char new_line[MAX_LINE_LENGTH];
    sprintf(new_line, "%02d:%02d %s", timer.minutes, timer.seconds, player_name);


    // Ouvrir le fichier en mode lecture
    char *file_path = get_file_path(difficulty);
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Créer un fichier temporaire pour stocker les modifications
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    bool file_empty = true;
    bool line_inserted = false;
    // Lire et modifier le contenu du fichier ligne par ligne
    char line[MAX_LINE_LENGTH];  // Taille de la ligne parcourue
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(new_line, line, 5) < 0 && !line_inserted) {
            fputs(new_line, temp_file);
            fputs("\n", temp_file);
            fputs(line, temp_file);
            line_inserted = true;
        } else {
            fputs(line, temp_file);
        }
        file_empty = false;
    }
    if (!line_inserted) {
        // Si la nouvelle ligne doit être ajoutée à la fin du fichier
        //fprintf(tempFile, "%s", nouvelle_ligne);
        if (!file_empty) {
            fputs("\n", temp_file);
        }
        fputs(new_line, temp_file);
    }

    // Fermer les fichiers
    fclose(file);
    fclose(temp_file);

    // Supprimer l'ancien fichier et renommer le fichier temporaire
    remove(file_path);
    rename("temp.txt", file_path);

}

void show_leaderboard(Difficulty difficulty) {
    // Ouvrir le fichier en mode lecture
    char *file_path = get_file_path(difficulty);
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];  // Taille de la ligne
    int max_name_length = 0; // Initialisation de la longueur maximale du nom du joueur à 0

    bool file_empty = true;

    while (fgets(line, sizeof(line), file) != NULL) {
        char timer[6]; // Suppose une longueur maximale de 5 pour le chronomètre (plus 1 pour le caractère nul)
        char player_name[MAX_NAME_LENGTH + 1]; // Pour stocker le nom du joueur

        // Utilisation de sscanf pour extraire les deux parties de la ligne
        sscanf(line, "%5s %49[^\n]", timer, player_name);

        // Vérification de la longueur du nom du joueur
        int name_length = strlen(player_name);
        if (name_length > max_name_length) {
            max_name_length = name_length; // Mise à jour de la longueur maximale si nécessaire
        }

        file_empty = false;
    }

    if (file_empty) {
        return;
    }

    switch (difficulty) {
        case EASY:
            printf("EASY\n");
            break;
        case MEDIUM:
            printf("MEDIUM\n");
            break;
        case HARD:
            printf("HARD\n");
            break;
        default:
            printf("EASY\n");
            break;
    }

    max_name_length = (int)strlen("Player") > max_name_length ? (int)strlen("Player") : max_name_length;
    max_name_length = max_name_length + 5;
    printf("%-10s %-*s %-10s\n", "Rank", max_name_length, "Player", "Timer");

    // Retourner au début du fichier pour afficher le scoreboard
    rewind(file);

    int ranking = 1;
    // Lire et afficher le contenu du fichier ligne par ligne
    while (fgets(line, sizeof(line), file) != NULL) {
        char timer[6]; // Suppose une longueur maximale de 5 pour le chronomètre (plus 1 pour le caractère nul)
        char player_name[MAX_NAME_LENGTH + 1]; // Pour stocker le nom du joueur

        // Utilisation de sscanf pour extraire les deux parties de la ligne
        sscanf(line, "%5s %49[^\n]", timer, player_name);

        // Affichage aligné en fonction de la longueur maximale du nom du joueur
        printf("%-10d %-*s %-10s\n", ranking, max_name_length, player_name, timer);
        ranking++;
    }
    printf("\n");

    // Fermer le fichier
    fclose(file);
}