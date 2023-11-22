#ifndef CONSTANTS_H
#define CONSTANTS_H

#define HEIGHT 5

//X est le nombre de Z en largeur nécessaire pour une barre du X
#if HEIGHT % 2
#define X (HEIGHT - 1)
#else
#define X (HEIGHT - 2)
#endif

#define WIDTH (3 * X)

#define Z (2 * X * HEIGHT)

#define PROBA 0.1

#endif