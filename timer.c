#include "timer.h"

#include <stdio.h>

Timer init_timer() {
    Timer timer = {0, 0};
    
    return timer;
}

Timer get_timer(int seconds) {
    Timer timer;

    if (seconds < 3600) {
        timer.minutes = seconds / 60;
        timer.seconds = seconds - (timer.minutes * 60);
    } else {
        timer.minutes = 59;
        timer.seconds = 59;
    }

    return timer;
}

void show_timer(Timer timer) {
    printf("Timer : %02d:%02d", timer.minutes, timer.seconds);
}