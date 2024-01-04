#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banner.h"
#include "menu.h"
#include "difficulty.h"
#include "game.h"
#include "leaderboard.h"

int main() {
    // Initialisation du générateur de nombres aléatoires
    srand((unsigned int)time(NULL));

    // Boucle principale du jeu
    while (1) {
        // Efface l'écran et affiche la bannière du jeu
        system("clear");
        show_banner("banners/minesweeper.txt");

        // Affiche le menu et récupère le choix de l'utilisateur
        show_menu();
        int choice = select_menu();
        system("clear");
        
        // Gestion des choix du menu
        switch (choice) {
            case MENU_PLAY_GAME:
                // Affiche le menu de sélection de difficulté et récupère la difficulté choisie
                show_difficulty();
                Difficulty difficulty = select_difficulty();
                system("clear");
                
                // Démarre le jeu avec la difficulté choisie
                play_game(difficulty);
                printf("\033[33m[Waiting]\033[0m Press enter to return to the menu...");
                while (getchar() != '\n');
                break;
            
            case MENU_LEADERBOARD:
                // Affiche le classement pour chaque niveau de difficulté
                show_banner("banners/leaderboard.txt");
                show_leaderboard(EASY);
                show_leaderboard(MEDIUM);
                show_leaderboard(HARD);
                printf("\033[33m[Waiting]\033[0m Press enter to return to the menu...");
                while (getchar() != '\n');
                break;
            
            case MENU_QUIT:
                // Quitte le jeu
                show_banner("banners/minesweeper.txt");
                printf("Hope you enjoyed the game! Goodbye!\n");
                return 0;
            
            default:
                break;
        }
    }
}