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

void clear_screen() {
    #ifdef _WIN32
        system("cls"); // Effacer l'écran dans une fenêtre de commande Windows (cmd)
    #else
        system("clear"); // Effacer l'écran dans un terminal Linux / UNIX
    #endif
}

bool check_win(Board board) {
    return board.unmined_revealed_cells == board.unmined_cells;
}

bool check_loss(Board board) {
    return board.loss;
}

Move select_move() {
    char input[100];

    while (1) {
        printf("\033[33m[Waiting]\033[0m Move : ");
        fgets(input, sizeof(input), stdin);

        // Supprimer le saut de ligne de la fin de la chaîne
        input[strcspn(input, "\n")] = '\0';

        // Vérifier si la saisie contient uniquement des chiffres ou des lettres
        int valid_input = 1;
        for (size_t i = 0; i < strlen(input); i++) {
            if (!isalnum(input[i])) {
                valid_input = 0;
                break;
            }
        }

        // Vérifier si le troisième caractère est 'R', 'S' ou 'U'
        if (valid_input) {
            if (toupper(input[2]) != 'R' && toupper(input[2]) != 'S' && toupper(input[2]) != 'U') {
                printf("\033[31m[Error]\033[0m Invalid move.\n");
            } else {
                break;
            }
        } else {
            printf("\033[31m[Error]\033[0m Invalid move.\n");
        }
    }

    Move move;
    move.action = toupper(input[2]);
    for (int i = 0; i < 36; i++) {
        if (BOARD_COORDINATES[i] == toupper(input[0])) {
            move.position.i = i;
            break;
        }
    }
    for (int j = 0; j < 36; j++) {
        if (BOARD_COORDINATES[j] == toupper(input[1])) {
            move.position.j = j;
            break;
        }
    }
    
    return move;
}

bool check_valid_move(Board board, Move move) {
    Position position = move.position;
    if (position.i < 0 || position.j < 0 || position.i >= board.height || position.j >= board.width) {
        printf("\033[31m[Error]\033[0m Position outside the board.\n");
        return false;
    }
    State state = board.cells[position.i][position.j].state;
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
    return true;
}

void reminder() {
    printf("\033[34m[Reminder]\033[0m\n");
    printf("The move 17R means you want to (R)eveal cell [raw=1, column=7].\n");
    printf("The move 17S means you want to (S)et a flag in cell [raw=1, column=7].\n");
    printf("The move 17U means you want to (U)nset the flag in cell [raw=1, column=7].\n");
    printf("Case is not important (upper or lower case).\n");
    printf("Timer will start after your first move.\n\n");
}

void show_remaining_mines(Board board) {
    printf("Remaining mines : %d", board.remaining_mines);
}

void play_game(Difficulty difficulty) {
    bool win;
    bool loss;
    Board board;
    init_board(&board, difficulty);
    reminder();

    // Variables pour stocker le temps de début et de fin
    time_t start, end;
    bool first_move = true;

    Timer timer = init_timer();

    do {
        printf("\033[32m[Info]\033[0m ");
        show_remaining_mines(board);
        if (!first_move) {
            printf("    ");
            show_timer(timer);
        }
        printf("\n\n");
        show_board(board, false);
        Move move;
        do {
            move = select_move();
        } while (!check_valid_move(board, move));
        update_board(&board, move);
        win = check_win(board);
        loss = check_loss(board);

        if (first_move) {
            start = time(NULL);
            first_move = false;
        } else {
            end = time(NULL);
            int time_elapsed = (int)difftime(end, start);
            timer = get_timer(time_elapsed);
        }

        clear_screen();
    } while (!win && !loss);

    printf("\033[32m[Info]\033[0m ");
    show_timer(timer);
    printf("\n\n");
    show_board(board, true);
    
    if (win) {
        show_banner("banners/win.txt");
        const char* player_name = select_player_name();
        archive_game(player_name, difficulty, timer, board);
        update_leaderboard(player_name, difficulty, timer);
        printf("Congratulations %s, you've earned your place on the leaderboard!\n\n", player_name);
    }
    if (loss) {
        show_banner("banners/lose.txt");
    }
    free_board(&board);
}

const char* select_player_name() {
    static char player_name[50];
    
    printf("\033[33m[Waiting]\033[0m Please enter your name : ");
    fgets(player_name, sizeof(player_name), stdin); // Récupère la saisie de l'utilisateur avec les espaces

    // Supprime le caractère de saut de ligne '\n' à la fin, s'il est présent
    size_t len = strlen(player_name);
    if (len > 0 && player_name[len - 1] == '\n') {
        player_name[len - 1] = '\0';
    }

    return player_name;
}

void archive_game(const char *player_name, Difficulty difficulty, Timer timer, Board board) {
    char full_path[50]; // Taille suffisante pour stocker la chaîne complète "archives/YYYY-MM-DD_hh-mm-ss"
    strcpy(full_path, "archives/"); // Copie la chaîne "archives/" dans full_path
    strcat(full_path, get_timestamp()); // Concatène le résultat retourné par get_timestamp()
    
    FILE *file;
    file = fopen(full_path, "w");
    
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Écrit les données dans le fichier file.txt
    fprintf(file, "Player : %s\n", player_name);

    fprintf(file, "Difficulty : ");
    switch (difficulty) {
        case EASY:
            fprintf(file, "Easy");
            break;
        case MEDIUM:
            fprintf(file, "Medium");
            break;
        case HARD:
            fprintf(file, "Hard");
            break;
        // Autres cas de difficulté
        default:
            fprintf(file, "Easy");
    }
    fprintf(file, " [Height : %d, Mines : %d]\n", board.height, board.mines);

    fprintf(file, "Timer : %02d:%02d\n\n", timer.minutes, timer.seconds);

    const char* board_string = get_board_string(board, true, false);
    fprintf(file, "%s", board_string);

    fclose(file); // Ferme le fichier après avoir écrit les données
}

const char* get_timestamp() {
    static char timestamp[20]; // Tableau de caractères pour stocker la chaîne de date et heure
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Formatage de la date et de l'heure selon le format spécifié
    sprintf(timestamp, "%04d-%02d-%02d_%02d-%02d-%02d",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    return timestamp;
}