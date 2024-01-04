#include "leaderboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_file_path(Difficulty difficulty) {
    switch (difficulty) {
        // Selon la difficulté, retourne le chemin d'accès au fichier correspondant
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
    // Obtient le chemin d'accès au fichier de classement pour la difficulté spécifiée
    const char *file_path = get_file_path(difficulty);
    
    // Ouvre le fichier de classement en mode lecture
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Crée un fichier temporaire pour stocker les modifications
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char new_line[MAX_LINE_LENGTH];
    snprintf(new_line, sizeof(new_line), "%02d:%02d %s", timer.minutes, timer.seconds, player_name);

    // Vérifie si le fichier est vide
    bool file_empty = true;
    if (fgets(line, sizeof(line), file) != NULL) {
        file_empty = false;
        rewind(file); // Retourne au début du fichier pour commencer la lecture
    }

    // Lit et modifie le contenu du fichier ligne par ligne
    bool line_inserted = false;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (!line_inserted && strncmp(new_line, line, 5) < 0) {
            // Insère la nouvelle ligne au bon endroit dans le fichier temporaire
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

    // Ferme les fichiers
    fclose(file);
    fclose(temp_file);

    // Supprime l'ancien fichier et renomme le fichier temporaire
    remove(file_path);
    rename("temp.txt", file_path);
}

void show_leaderboard(Difficulty difficulty) {
    // Obtient le chemin d'accès au fichier de classement pour la difficulté spécifiée
    const char *file_path = get_file_path(difficulty);

    // Ouvre le fichier de classement en mode lecture
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    // Vérifie si le fichier est vide
    bool file_empty = true;
    if (fgets(line, sizeof(line), file) != NULL) {
        file_empty = false;
        rewind(file); // Retourne au début du fichier pour commencer la lecture
    }
    if (file_empty) {
        return;
    }

    // Affiche le titre du classement en fonction de la difficulté
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
    
    int max_name_length = 0; // Longueur maximale du nom du joueur initialisée à 0
    // Calcule la longueur maximale du nom du joueur dans le fichier de classement
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

    // Ajuste la longueur maximale du nom du joueur pour tenir compte du titre "Player" et de la marge
    max_name_length = (int)strlen("Player") > max_name_length ? (int)strlen("Player") : max_name_length;
    max_name_length = max_name_length + 5;

    // Affiche le titre du classement avec une mise en forme alignée
    printf("%-10s %-*s %-10s\n", "Rank", max_name_length, "Player", "Timer");

    // Retourner au début du fichier pour afficher le scoreboard
    rewind(file);

    int ranking = 1; // Rang du classement initialisé à 1
    // Lit et affiche le contenu du fichier ligne par ligne
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

    // Ferme le fichier
    fclose(file);
}