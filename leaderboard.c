#include "leaderboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_file_path(Difficulty difficulty) {
    switch (difficulty) {
        case EASY:
            return "leaderboard/easy.txt";
        case MEDIUM:
            return "leaderboard/medium.txt";
        case HARD:
            return "leaderboard/hard.txt";
        default:
            return "leaderboard/easy.txt"; // Valeur par défaut au cas où
    }
}

void update_leaderboard(char *player_name, Difficulty difficulty, Timer timer) {
    // Ouvrir le fichier en mode lecture
    const char *file_path = get_file_path(difficulty);
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

    char line[MAX_LINE_LENGTH];
    char new_line[MAX_LINE_LENGTH];
    snprintf(new_line, sizeof(new_line), "%02d:%02d %s", timer.minutes, timer.seconds, player_name);

    // Vérifier si le fichier est vide
    bool file_empty = true;
    if (fgets(line, sizeof(line), file) == NULL) {
        file_empty = true;
    } else {
        file_empty = false;
        rewind(file); // Retourner au début du fichier pour commencer la lecture
    }

    // Lire et modifier le contenu du fichier ligne par ligne
    bool line_inserted = false;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (!line_inserted && strncmp(new_line, line, 5) < 0) {
            fputs(new_line, temp_file);
            fputs("\n", temp_file);
            fputs(line, temp_file);
            line_inserted = true;
        } else {
            fputs(line, temp_file);
        }
    }

    if (!line_inserted) {
        // Si la nouvelle ligne doit être ajoutée à la fin du fichier
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
    const char *file_path = get_file_path(difficulty);
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    // Vérifier si le fichier est vide
    bool file_empty = true;
    if (fgets(line, sizeof(line), file) == NULL) {
        file_empty = true;
    } else {
        file_empty = false;
        rewind(file); // Retourner au début du fichier pour commencer la lecture
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
    
    int max_name_length = 0; // Initialisation de la longueur maximale du nom du joueur à 0
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
    }

    max_name_length = (int)strlen("Player") > max_name_length ? (int)strlen("Player") : max_name_length;
    max_name_length = max_name_length + 5;
    printf("%-10s %-*s %-10s\n", "Rank", max_name_length, "Player", "Timer");

    // Retourner au début du fichier pour afficher le scoreboard
    rewind(file);

    // Lire et afficher le contenu du fichier ligne par ligne
    int ranking = 1;
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