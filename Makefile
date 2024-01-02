CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = main.c banner.c board.c difficulty.c game.c leaderboard.c menu.c timer.c

all: $(SRC)
	$(CC) $(CFLAGS) $^ -o prog

clean:
	rm -f prog