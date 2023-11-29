#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


int get_time(){
    // Obtenir le temps actuel en secondes depuis le lancement du programme
    time_t start_time = time(NULL);
    printf("Veuillez appuyer sur Entrée pour stopper le timer. \n");
    // Lire le caractère entrée par l'utilisateur
    getchar();
    // Obtenir le temps actuel en secondes depuis le lancement du programme
    time_t end_time = time(NULL);
    // Calculer le nombre de secondes écoulées
    int seconds_elapsed = end_time - start_time;
    // Afficher le nombre de secondes écoulées
    printf("Il s'est écoulé %d secondes. \n", seconds_elapsed);
    return seconds_elapsed;
}

void print_timer(int x) {
    int h,m,s;
    h = x / 3600;
    m = (x % 3600) / 60;
    s = (x % 3600) % 60;
    printf("[Info] Timer : %02d:%02d:%02d\n", h,m,s);
}

int main() {
    int x = 12345;
    print_timer(x);
    return 0;
}

