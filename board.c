#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tableau de caractères représentant les coordonnées du plateau de jeu
const char BOARD_COORDINATES[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

// Hauteur du plateau pour chaque niveau de difficulté
#define EASY_HEIGHT 5
#define MEDIUM_HEIGHT 9
#define HARD_HEIGHT 13

// Probabilité de présence des mines pour chaque niveau de difficulté
#define EASY_MINES_PROBA 0.1
#define MEDIUM_MINES_PROBA 0.15
#define HARD_MINES_PROBA 0.2

void init_board(Board *board, Difficulty difficulty) {
    // Initialise la hauteur du plateau et la probabilité de mines selon la difficulté choisie
    switch (difficulty) {
        case EASY:
            board->height = EASY_HEIGHT;
            board->mines_proba = EASY_MINES_PROBA;
            break;
        case MEDIUM:
            board->height = MEDIUM_HEIGHT;
            board->mines_proba = MEDIUM_MINES_PROBA;
            break;
        case HARD:
            board->height = HARD_HEIGHT;
            board->mines_proba = HARD_MINES_PROBA;
            break;
        default:
            board->height = EASY_HEIGHT;
            board->mines_proba = EASY_MINES_PROBA;
            break;
    }

    // Calcul de la largeur et d'autres paramètres du plateau en fonction de la hauteur
    if (board->height % 2) {
        board->x_bar_width = board->height - 1;
    } else {
        board->x_bar_width = board->height - 2;
    }
    board->width = 3 * board->x_bar_width;
    board->playable_cells = 2 * board->x_bar_width * board->height;
    board->mines = (int)(board->mines_proba * board->playable_cells + 0.5);
    board->remaining_mines = board->mines;
    board->unmined_cells = board->playable_cells - board->mines;
    board->unmined_revealed_cells = 0;
    board->loss = false;

    // Alloue la mémoire nécessaire pour le plateau
    alloc_board(board);

    // Initialise les cellules du plateau
    init_cells(board);

    // Place les mines aléatoirement sur le plateau
    place_mines(board);
}

void alloc_board(Board *board) {
    // Alloue de la mémoire pour les cellules du plateau (tableau 2D de Cell)
    board->cells = (Cell**)malloc(board->height * sizeof(Cell*));
    for (int i = 0; i < board->height; i++) {
        board->cells[i] = (Cell*)malloc(board->width * sizeof(Cell));
    }

    // Alloue de la mémoire pour les positions des cellules jouables
    board->playable_cells_positions = (Position*)malloc(board->playable_cells * sizeof(Position));
}

void free_board(Board *board) {
    // Libère la mémoire allouée pour chaque ligne du plateau
    for (int i = 0; i < board->height; i++) {
        free(board->cells[i]);
    }
    // Libère la mémoire allouée pour le tableau de lignes (tableau 2D)
    free(board->cells);

    // Libère la mémoire allouée pour stocker les positions des cellules jouables
    free(board->playable_cells_positions);
}

void init_cells(Board *board) {
    int count = 0; // Compteur pour les positions des cellules jouables
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            // Vérifie si la cellule est jouable en fonction de ses coordonnées
            bool x_top = (i <= board->height / 2);
            bool x_top_left = (j >= i) && (j < i + board->x_bar_width);
            bool x_top_right = (j >= board->width - (i + board->x_bar_width)) && (j < board->width - i);
            bool x_bottom = (i >= board->height / 2);
            bool x_bottom_left = (j >= board->height - i - 1) && (j < board->height - i - 1 + board->x_bar_width);
            bool x_bottom_right = (j >= board->width - (board->height - i - 1 + board->x_bar_width)) && (j < board->width - (board->height - i - 1));
            
            bool is_playable_cell = (x_top && (x_top_left || x_top_right)) || (x_bottom && (x_bottom_left || x_bottom_right));
            
            // Initialise l'état de la cellule et ses caractéristiques
            if (is_playable_cell) {
                board->cells[i][j].state = HIDDEN;
                board->cells[i][j].is_mine = false;
                board->cells[i][j].adjacent_mines = 0;
                
                // Stocke les positions des cellules jouables
                board->playable_cells_positions[count].i = i;
                board->playable_cells_positions[count].j = j;
                count++;
            } else {
                board->cells[i][j].state = OUT;
            }
        }
    }
}

void place_mines(Board *board) {
    int count = 0; // Compteur pour le nombre de mines placées
    while (count < board->mines) {
        int random = rand() % board->playable_cells; // Génère un nombre aléatoire entre 0 et le nombre de cellules jouables
        Position position = board->playable_cells_positions[random]; // Récupère une position de cellule jouable aléatoire
        
        // Vérifie si la cellule à la position choisie n'est pas déjà une mine
        if (!board->cells[position.i][position.j].is_mine) {
            board->cells[position.i][position.j].is_mine = true; // Place une mine dans la cellule
            count++; // Incrémente le compteur de mines placées
            
            // Met à jour le nombre de mines adjacentes pour les cellules voisines
            Neighbours neighbours = get_neighbours(*board, position);
            for (int k = 0; k < neighbours.number; k++) {
                board->cells[neighbours.positions[k].i][neighbours.positions[k].j].adjacent_mines++;
            }
        }
    }
}

void update_board(Board *board, Move move) {
    Position position = move.position; // Récupère la position de la cellule concernée par le coup joué
    switch (move.action) {
        case 'S': // Si le joueur place un drapeau
            board->cells[position.i][position.j].state = FLAGGED; // Marque la cellule avec un drapeau
            board->remaining_mines--; // Décrémente le nombre de mines restantes sur le plateau
            break;
        case 'U': // Si le joueur enlève un drapeau
            board->cells[position.i][position.j].state = HIDDEN; // Cache à nouveau la cellule
            board->remaining_mines++; // Incrémente le nombre de mines restantes
            break;
        case 'R': // Si le joueur révèle une cellule
            board->cells[position.i][position.j].state = REVEALED; // Révèle la cellule
            if (!board->cells[position.i][position.j].is_mine) { // Si la cellule n'est pas une mine
                board->unmined_revealed_cells++; // Incrémente le compteur des cellules non minées révélées
                reveal_neighbouring_cells_recursively(board, position); // Déclenche la révélation récursive des cellules voisines
            } else {
                board->loss = true; // Défaite si la cellule révélée est une mine
            }
            break;
        default:
            break;
    }
}

void reveal_neighbouring_cells_recursively(Board *board, Position position) {
    Neighbours neighbours = get_neighbours(*board, position);
    int adjacent_mines = board->cells[position.i][position.j].adjacent_mines;
    
    // Si la cellule révélée n'a aucune mine adjacente
    if (adjacent_mines == 0) {
        // Parcours des cellules voisines pour les révéler récursivement si elles sont cachées
        for (int k = 0; k < neighbours.number; k++) {
            if (board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state == HIDDEN) {
                board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state = REVEALED;
                board->unmined_revealed_cells++;
                reveal_neighbouring_cells_recursively(board, neighbours.positions[k]);
            }
        }
    } else { // Si la cellule a des mines adjacentes
        int flagged_neighbours = 0;
        for (int k = 0; k < neighbours.number; k++) {
            if (board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state == FLAGGED) {
                flagged_neighbours++;
            }
        }

        // Si le nombre de mines adjacente est égal au nombre de drapeaux adjacents
        if (flagged_neighbours == adjacent_mines) {
            // Révélation récursive des cellules voisines cachées
            for (int k = 0; k < neighbours.number; k++) {
                if (board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state == HIDDEN) {
                    board->cells[neighbours.positions[k].i][neighbours.positions[k].j].state = REVEALED;
                    if (!board->cells[neighbours.positions[k].i][neighbours.positions[k].j].is_mine) {
                        board->unmined_revealed_cells++;
                        reveal_neighbouring_cells_recursively(board, neighbours.positions[k]);
                    } else {
                        board->loss = true;
                    }
                }
            }
        }
    }
}

Neighbours get_neighbours(Board board, Position position) {
    Neighbours neighbours;
    neighbours.number = 0;
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i != 0 || j != 0) { // Évite de considérer la cellule centrale
                int k = position.i + i;
                int l = position.j + j;
                
                // Vérifie si la position (k, l) est dans les limites du plateau
                if (k >= 0 && k < board.height && l >= 0 && l < board.width) {
                    // Vérifie si la cellule n'est pas à l'extérieur (OUT)
                    if (board.cells[position.i][position.j].state != OUT) {
                        Position neighbour_position = {k, l};
                        neighbours.positions[neighbours.number] = neighbour_position; // Ajoute la position du voisin à la liste
                        neighbours.number++; // Incrémente le nombre de voisins
                    }
                }
            }
        }
    }
    
    return neighbours; // Renvoie la liste des positions des voisins
}

void show_board(Board board, bool show_mines, bool coloured) {
    // Affichage des coordonnées des colonnes
    printf("   ");
    for (int j = 0; j < board.width; j++) {
        printf("  %c ", BOARD_COORDINATES[j]);
    }
    printf("\n");
    
    // Affichage du contenu du plateau de jeu
    for (int i = 0; i < board.height; i++) {
        // Délimitation des lignes du tableau
        printf("   ");
        if (i == 0) {
            for (int j = 0; j < board.width; j++) {
                if (j == 0) {
                    printf("\u250C\u2500\u2500\u2500");
                } else {
                    printf("\u252C\u2500\u2500\u2500");
                }
            }
            printf("\u2510 \n");
        } else {
            for (int j = 0; j < board.width; j++) {
                if (j == 0) {
                    printf("\u251C\u2500\u2500\u2500");
                } else {
                    printf("\u253C\u2500\u2500\u2500");
                }
            }
            printf("\u2524 \n");
        }
        
        // Affichage des coordonnées des lignes
        printf(" %c ", BOARD_COORDINATES[i]);

        // Affichage des cellules de chaque ligne
        for (int j = 0; j < board.width; j++) {
            Cell cell = board.cells[i][j];
            if (cell.state == OUT) {
                printf("\u2502 \u2592 ");
            }
            if (cell.state == FLAGGED) {
                if (!show_mines || cell.is_mine) {
                    printf("\u2502 \u2691 ");
                } else {
                    printf("\u2502 X ");
                }
            }
            if (cell.state == HIDDEN) {
                if (!show_mines || !cell.is_mine) {
                    printf("\u2502   ");
                } else {
                    printf("\u2502 \u25A0 ");
                }
            }
            if (cell.state == REVEALED) {
                if (!show_mines || !cell.is_mine) {
                    printf("\u2502 %s ", format_adjacent_mines(cell.adjacent_mines, coloured));
                } else {
                    printf("\u2502 \u25A2 ");
                }
            }
        }

        // Réaffichage des coordonnées des lignes à droite du tableau
        printf("\u2502 %c \n", BOARD_COORDINATES[i]);
    }

    // Lignes horizontales inférieures du tableau
    printf("   ");
    for (int j = 0; j < board.width; j++) {
        if (j == 0) {
            printf("\u2514\u2500\u2500\u2500");
        } else {
            printf("\u2534\u2500\u2500\u2500");
        }
    }
    printf("\u2518 \n");
    
    // Réaffichage des coordonnées des colonnes en bas du tableau
    printf("   ");
    for (int j = 0; j < board.width; j++) {
        printf("  %c ", BOARD_COORDINATES[j]);
    }
}

const char* format_adjacent_mines(int adjacent_mines, bool coloured) {
    static char output[20];

    // Si la cellule n'a pas de mines adjacentes, affiche un point
    if (adjacent_mines == 0) {
        return "\u00B7";
    }

    // Si la couleur est activée, applique une couleur différente en fonction du nombre de mines adjacentes
    if (coloured) {
        if (adjacent_mines == 1) {
            sprintf(output, "\033[34m%d\033[0m", adjacent_mines); // Bleu pour 1 mine adjacente
        } else if (adjacent_mines == 2) {
            sprintf(output, "\033[32m%d\033[0m", adjacent_mines); // Vert pour 2 mines adjacentes
        } else if (adjacent_mines == 3) {
            sprintf(output, "\033[33m%d\033[0m", adjacent_mines); // Jaune pour 3 mines adjacentes
        } else {
            sprintf(output, "\033[31m%d\033[0m", adjacent_mines); // Rouge pour 4 mines adjacentes ou plus
        }
    } else { // Si la couleur n'est pas activée, affiche simplement le nombre de mines adjacentes
        sprintf(output, "%d", adjacent_mines);
    }

    return output; // Renvoie la chaîne formatée pour affichage
}