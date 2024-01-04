#include "timer.h"
#include <stdio.h>

Timer init_timer() {
    Timer timer = {0, 0}; // Initialise un timer à 00:00 (0 minute et 0 seconde)
    return timer; // Renvoie le timer initialisé
}

Timer get_timer(int seconds) {
    Timer timer;

    if (seconds < 3600) {
        // Convertit les secondes en minutes et secondes
        timer.minutes = seconds / 60;
        timer.seconds = seconds % 60;
    } else {
        // Limite le temps maximal à 59:59 (59 minutes et 59 secondes)
        timer.minutes = 59;
        timer.seconds = 59;
    }

    return timer; // Renvoie la structure Timer avec le temps correspondant
}

void show_timer(Timer timer) {
    // Affiche le temps sous la forme MM:SS (minutes:secondes)
    printf("Timer : %02d:%02d", timer.minutes, timer.seconds);
}