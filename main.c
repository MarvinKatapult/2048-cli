
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <cterm.h>

#define BOARD_GEOMETRY  4

static unsigned long board[BOARD_GEOMETRY][BOARD_GEOMETRY] = { 
    { 0, 0, 0, 0 },
    { 0, 2, 2, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

CT_Color getColForNum(int num) {
    
    int col = num % 12;
    switch (col) {
        case 0:
            return CT_Red;
        case 1:
            return CT_Green;
        case 2:
            return CT_Yellow;
        case 3:
            return CT_Blue;
        case 4:
            return CT_Magenta;
        case 5:
            return CT_Cyan;
        case 6:
            return CT_White;
        case 7:
            return CT_LRed;
        case 8:
            return CT_LGreen;
        case 9:
            return CT_LYellow;
        case 10:
            return CT_LBlue;
        case 11:
            return CT_LMagenta;
        case 12:
            return CT_LCyan;
        case 13:
            return CT_LWhite;
    }

    return CT_White;
}

void printBoard() {
    for (int i = 0; i < BOARD_GEOMETRY; i++) {
        for (int j = 0; j < BOARD_GEOMETRY; j++) {
            long val = board[i][j];
            if (true || val != 0) {
                char buf[8] = { };
                sprintf(buf, "%7ld", val);
                buf[7] = '\0';
                putStrExt(buf, getColForNum(val), CT_Black);
            }
        }
        moveCursor(1, ((i+1) * 4) + 1);
    }

    fflush(stdout);
}

void getDirForInp(unsigned char inp, int * x, int * y) {
    switch (inp) {
        case 'w':
            *x = 0;
            *y = -1;
            break;
        case 's':
            *x = 0;
            *y = 1;
            break;
        case 'a':
            *x = -1;
            *y = 0;
            break;
        case 'd':
            *x = 1;
            *y = 0;
            break;
    }
}

void move(int x, int y) {

    bool changed = false;
    for (int i = 0; i < BOARD_GEOMETRY; i++) {
        for (int j = 0; j < BOARD_GEOMETRY; j++) {
            if ((x == -1 && j == 0) || (x == 1 && j == BOARD_GEOMETRY - 1)) continue;
            if ((y == -1 && i == 0) || (y == 1 && i == BOARD_GEOMETRY - 1)) break;

            unsigned long * start = &board[i][j];
            unsigned long * dest  = &board[i+y][j+x];

            if (*start == 0) continue;

            if (*dest == 0) {
                *dest = *start;
                *start = 0;
                changed = true;
            }
            else if (*start == *dest) {
                *dest *= 2;
                *start = 0;
                changed = true;
            }
        }
    } 
    if (changed) move(x, y);
}

bool spawnNewVals() {
	unsigned long * zeroes[16] = {};
	int count = 0;
	for (int i = 0; i < BOARD_GEOMETRY; i++) {
		for (int j = 0; j < BOARD_GEOMETRY; j++) {
			unsigned long val = board[i][j];
			if (val == 0) {
				zeroes[count++] = &board[i][j];
			}
		}
	}

	if (count == 0) {
		return false;
	}

	int new_loc = rand() % count;
	*zeroes[new_loc] = 2;
	return true;
}

int main() {

	srand(time(NULL));
    enableRawMode();

    while (1) {
        clearScreen();
        moveCursor(1, 1);
        printBoard();
        unsigned char inp = readTermInput();
        if (inp == 'q') break;
        if (inp == 'w' || inp == 's' || inp == 'a' || inp == 'd') {
            int x, y;
            getDirForInp(inp, &x, &y);
            move(x, y);
            if (!spawnNewVals()) {
				printf("You lost!\n");
				disableRawMode();
				exit(EXIT_SUCCESS);
			}
        }
    }

    return 0;
}
