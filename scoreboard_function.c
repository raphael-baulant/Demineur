#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"
#include <time.h>
#include <string.h>
#include <unistd.h>

#define MAX_SCORES 100

// Structure pour stocker les scores et les noms d'utilisateur
struct Record {
    char username[50];
    int time;
};

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

int getNumberOfRecordsByDifficulty(int targetDifficulty){
    int lines = 0;
    int difficulty;

    FILE *fichier = fopen("record.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    while (fscanf(fichier, "%d:%*49[^:]:%*d", &difficulty) == 1) {
        // Afficher les valeurs sous la forme "[difficulty] username - timer"
        if(difficulty == targetDifficulty)
            lines++;
    }

    return lines;
}

int compareRecords(const void *a, const void *b) {
    return ((struct Record *)a)->time - ((struct Record *)b)->time;
}

struct Record* getSortedRecords(int targetDifficulty) {
    
    int lines = getNumberOfRecordsByDifficulty(targetDifficulty);
    int difficulty;
    char username[50];
    int time;

    FILE *fichier = fopen("record.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    struct Record* RecordsList = malloc(sizeof(struct Record) * lines);

    // Réinitialiser le curseur du fichier
    fseek(fichier, 0, SEEK_SET);
    int i = 0;

    // Lire le fichier ligne par ligne
    while (fscanf(fichier, "%d:%49[^:]:%d", &difficulty, username, &time) == 3) {
        // Afficher les valeurs sous la forme "[difficulty] username - timer"
        if(difficulty == targetDifficulty){
            strcpy(RecordsList[i].username, username);
            RecordsList[i].time = time;
            i++;
        }
    }
    fclose(fichier);
    qsort(RecordsList, lines, sizeof(struct Record), compareRecords);
    return RecordsList;
}

void printBestRecord(int targetDifficulty){
    struct Record* sortedRecords = getSortedRecords(targetDifficulty);
    int numberOfRecords = getNumberOfRecordsByDifficulty(targetDifficulty);

    if(numberOfRecords>3){
        for(int i=0; i<3; i++){
            printf("    #%d %s %d\n",i,sortedRecords[i].username, sortedRecords[i].time);
        }
    }
    else{
        for(int i=0; i<numberOfRecords; i++){
            printf("    #%d %s %d\n",i,sortedRecords[i].username, sortedRecords[i].time);
        }
    }
}

int main() {
    int targetDifficulty = 18;
    printBestRecord(targetDifficulty);

    return 0;
}