#ifndef TIMER_H
#define TIMER_H

#include "structures.h"

Timer init_timer();
Timer get_timer(int seconds);
void show_timer(Timer timer);

#endif /* TIMER_H */