#ifndef TIMER_H
#define TIMER_H

#include "structures.h"

/*
 * Initialise un timer à 00:00.
 * Renvoie : un Timer initialisé.
 */
Timer init_timer();

/*
 * Retourne un timer avec une durée en secondes spécifiée.
 * Paramètre : seconds - la durée en secondes du timer.
 * Renvoie : un Timer initialisé avec la durée spécifiée.
 */
Timer get_timer(int seconds);

/*
 * Affiche le timer spécifié.
 * Paramètre : timer - le Timer à afficher.
 */
void show_timer(Timer timer);

#endif /* TIMER_H */