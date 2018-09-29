//
// Created by Nine_yota on 2018-09-29.
//

#include "drawBoard.h"
#include "icld.h"


char drawing_board[BOARD_SIZE][BOARD_SIZE * CHAR_SIZE * 2] = {
        "┏━┳━┳━┳━┳━┳━┳━┳━┳━┳━┳━┳━┳━┳━┓",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┣━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━╋━┫",
        "┗━┻━┻━┻━┻━┻━┻━┻━┻━┻━┻━┻━┻━┻━┛"
};
char white_piece[] = "●";
char black_piece[] = "○";
//0==null 1==white -1==black
extern int status_board[BOARD_SIZE][BOARD_SIZE];

void output_board() {
    printf(" ");
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        printf("%d", i);
        if (i < 10)
            printf(" ");
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%c", 'A' + i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            //CHAR_SIZE-1: avoid copying the end symbol of string.
            if (status_board[i][j] == 1) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = white_piece[k];
                }
            } else if (status_board[i][j] == -1) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = black_piece[k];
                }
            }
        }
        printf("%s\n", drawing_board[i]);
    }
}
