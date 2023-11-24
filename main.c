#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "functions.h"

int main() {
    srand((unsigned int)time(NULL));

    Cell** grid = allocateGrid();
    initialiseGrid(grid);
    
    displayGrid(grid);

    Choice choice = choose(grid);

    //debbug avec print
    printf("Choix : %d %d %c\n", choice.position.i, choice.position.j, choice.action);

    freeGrid(grid);
    return 0;
}