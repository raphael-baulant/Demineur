#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "board.h"
#include "banner.h"
#include "timer.h"
#include "leaderboard.h"

void play_game(Difficulty difficulty) {
    // Initialisation du plateau
    Board board;
    init_board(&board, difficulty);
    
    // Chronomètre
    Timer timer = init_timer(); // Initialisation du chronomètre
    time_t start, end; // Variables pour stocker le temps de début et de fin
    bool first_move = true; // Le chronomètre commence après le premier coup

    // Variables pour stocker la victoire et la défaite
    bool win, loss;

    // Affichage du rappel des coups possibles
    reminder();

    // Boucle principale du jeu
    do {
        // Affichage des informations sur les mines restantes et le chronomètre (si le 1er coup a été joué)
        printf("\033[32m[Info]\033[0m ");
        show_remaining_mines(board);
        if (!first_move) {
            printf("    ");
            show_timer(timer);
        }
        printf("\n\n");

        // Affichage du plateau de jeu
        show_board(board, false, true);
        printf("\n\n");
        
        // Sélection du coup par le joueur et vérification de sa validité
        Move move;
        do {
            move = select_move();
        } while (!check_valid_move(board, move));
        
        // Mise à jour du plateau après le coup du joueur
        update_board(&board, move);
        
        // Vérification de la victoire ou défaite du joueur
        win = check_win(board);
        loss = check_loss(board);

        // Gestion du temps écoulé
        if (first_move) {
            start = time(NULL);
            first_move = false;
        } else {
            end = time(NULL);
            int time_elapsed = (int)difftime(end, start);
            timer = get_timer(time_elapsed);
        }

        // Effacement de l'écran pour la prochaine itération
        system("clear");
    } while (!win && !loss);

    // Affichage du chronomètre final et du plateau de jeu
    printf("\033[32m[Info]\033[0m ");
    show_timer(timer);
    printf("\n\n");
    show_board(board, true, true);
    printf("\n\n");

    // Traitement de la victoire ou de la défaite du joueur
    if (win) {
        show_banner("banners/win.txt");
        
        // Sélection du nom du joueur
        char* player_name = select_player_name();
        
        // Archivage de la partie
        archive_game(player_name, difficulty, timer, board);
        
        // Mise à jour du classement
        update_leaderboard(player_name, difficulty, timer);
        printf("Congratulations %s, you've earned your place on the leaderboard!\n\n", player_name);
        
        // Libération de la mémoire du nom du joueur
        free(player_name);
    } else {
        show_banner("banners/lose.txt");
    }
    
    // Libération de la mémoire allouée pour le plateau de jeu
    free_board(&board);
}

void reminder() {
    // Affichage du rappel des coups possibles
    printf("\033[34m[Reminder]\033[0m\n");
    printf("The move 17R means you want to (R)eveal cell [raw=1, column=7].\n");
    printf("The move 17S means you want to (S)et a flag in cell [raw=1, column=7].\n");
    printf("The move 17U means you want to (U)nset the flag in cell [raw=1, column=7].\n");
    printf("Case is not important (upper or lower case).\n");
    printf("Timer will start after your first move.\n\n");
}

void show_remaining_mines(Board board) {
    // Affiche le nombre de mines restantes sur le plateau de jeu
    printf("Remaining mines : %d", board.remaining_mines);
}

Move select_move() {
    char input[100];

    // Demande de saisir un coup jusqu'à ce qu'un coup valide soit entré
    while (1) {
        printf("\033[33m[Waiting]\033[0m Move : ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        // Vérifie si la saisie contient uniquement des chiffres ou des lettres
        bool valid_input = true;
        for (size_t i = 0; i < strlen(input); i++) {
            if (!isalnum(input[i])) {
                valid_input = false;
                break;
            }
        }

        // Vérifie si le troisième caractère est 'R', 'S' ou 'U'
        if (valid_input) {
            char action = toupper(input[2]);
            if (action != 'R' && action != 'S' && action != 'U') {
                printf("\033[31m[Error]\033[0m Invalid move.\n");
            } else {
                break; // Sort de la boucle si le coup est valide
            }
        } else {
            printf("\033[31m[Error]\033[0m Invalid move.\n");
        }
    }

    Move move;
    move.action = toupper(input[2]);

    // Récupère les coordonnées à partir des entrées de l'utilisateur
    move.position.i = get_coordinate(input[0]);
    move.position.j = get_coordinate(input[1]);
    
    return move;
}

int get_coordinate(char input) {
    for (int i = 0; i < BOARD_COORDINATES_LENGTH; i++) {
        if (toupper(input) == BOARD_COORDINATES[i]) {
            return i;
        }
    }

    // Valeur par défaut si l'entrée n'est pas valide
    return 0;
}

bool check_valid_move(Board board, Move move) {
    Position position = move.position;

    // Vérifie si la position est en dehors des limites du plateau
    if (position.i < 0 || position.j < 0 || position.i >= board.height || position.j >= board.width) {
        printf("\033[31m[Error]\033[0m Position outside the board.\n");
        return false;
    }
    
    // Obtient l'état de la case à la position donnée
    State state = board.cells[position.i][position.j].state;
    
    // Vérifie différents scénarios invalides en fonction de l'état actuel de la case
    if (state == OUT) {
        printf("\033[31m[Error]\033[0m Position outside the X.\n");
        return false;
    }
    if (state == REVEALED) {
        printf("\033[31m[Error]\033[0m Position already revealed.\n");
        return false;
    } 
    if (state == FLAGGED) {
        if (move.action == 'S') {
            printf("\033[31m[Error]\033[0m Position already flagged.\n");
            return false;
        }
        
        if (move.action == 'R') {
            printf("\033[31m[Error]\033[0m Impossible to reveal a flagged position.\n");
            return false;
        }
    }
    if (state == HIDDEN) {
        if (move.action == 'U') {
            printf("\033[31m[Error]\033[0m Position already unflagged.\n");
            return false;
        }
    }
    
    // Si aucune condition d'invalidité n'est rencontrée, le coup est valide
    return true;
}

bool check_win(Board board) {
    return board.unmined_revealed_cells == board.unmined_cells;
}

bool check_loss(Board board) {
    return board.loss;
}

char* select_player_name() {
    // Alloue dynamiquement de la mémoire pour stocker le nom du joueur
    char* player_name = (char*)malloc(MAX_NAME_LENGTH * sizeof(char));
    bool valid_input = false;

    // Boucle jusqu'à ce que la saisie soit valide
    while (!valid_input) {
        // Demande à l'utilisateur d'entrer son nom
        printf("\033[33m[Waiting]\033[0m Please enter your name: ");
        fgets(player_name, MAX_NAME_LENGTH, stdin); // Récupère la saisie de l'utilisateur avec les espaces

        // Supprime le caractère de saut de ligne '\n' à la fin, s'il est présent
        size_t len = strlen(player_name);
        if (len > 0 && player_name[len - 1] == '\n') {
            player_name[len - 1] = '\0';
        }

        // Vérifie si la saisie contient au moins un caractère et que le premier caractère est une lettre ou un chiffre
        if (strlen(player_name) > 0 && (isalpha(player_name[0]) || isdigit(player_name[0]))) {
            valid_input = true; // La saisie est valide, sortir de la boucle
        } else {
            printf("\033[31m[Error]\033[0m Invalid name.\n");
        }
    }

    return player_name;
}

void archive_game(char *player_name, Difficulty difficulty, Timer timer, Board board) {
    const int MAX_PATH_LENGTH = 100;
    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, MAX_PATH_LENGTH, "archives/");
    
    const char* timestamp = get_timestamp();
    strncat(full_path, timestamp, MAX_PATH_LENGTH - strlen(full_path) - 1);
    free((void*)timestamp);

    // Crée un fichier pour l'archive
    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        printf("Error creating temporary file.\n");
        return;
    }

    // Redirige la sortie standard (stdout) vers le fichier
    fflush(stdout);
    freopen(full_path, "w", stdout);

    // Écrit les détails du jeu dans le fichier d'archive
    printf("Player : %s\n", player_name);
    printf("Difficulty : ");
    switch (difficulty) {
        case EASY:
            printf("Easy");
            break;
        case MEDIUM:
            printf("Medium");
            break;
        case HARD:
            printf("Hard");
            break;
        default:
            printf("Easy");
            break;
    }
    printf(" [Height : %d, Mines : %d]\n", board.height, board.mines);
    printf("Timer : %02d:%02d\n\n", timer.minutes, timer.seconds);
    show_board(board, true, false);
    
    // Restaure la sortie standard vers la console
    fflush(stdout);
    freopen("/dev/tty", "w", stdout);

    // Ferme le fichier d'archive
    fclose(file);
}

const char* get_timestamp() {
    time_t rawtime;
    struct tm *timeinfo;
    char* timestamp = (char*)malloc(20 * sizeof(char)); // Allocation d'un espace mémoire pour le timestamp

    time(&rawtime); // Obtention du temps actuel en secondes depuis l'epoch
    timeinfo = localtime(&rawtime); // Conversion en structure tm pour obtenir la date et l'heure locales

    // Formatage de la date et de l'heure au format AAAA-MM-JJ_HH-MM-SS
    sprintf(timestamp, "%04d-%02d-%02d_%02d-%02d-%02d",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    return timestamp;
}