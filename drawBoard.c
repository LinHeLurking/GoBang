//
// Created by Nine_yota on 2018-09-29.
//

#include "drawBoard.h"

extern boardStatus status;
extern boardStatus dfs_status;
extern drop_record record[BOARD_SIZE * BOARD_SIZE + 5];

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
            if (status.board[i][j] == 1) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = white_piece[k];
                }
            } else if (status.board[i][j] == -1) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = black_piece[k];
                }
            }
        }
        printf("%s\n", drawing_board[i]);
    }

#ifdef PLAYER_SIDE_DEBUG
    printf("Last player: ");
    switch (record[status.steps].player) {
        case WHITE:
            printf("white\n");
            break;
        case BLACK:
            printf("black\n");
            break;
        default:
            printf("No one\n");
    }
#endif
}

void dfs_output_board() {
    printf(" ");
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        printf("%d", i);
        if (i < 10)
            printf(" ");
    }
    printf("\n");
    char dfs_drawing_board[BOARD_SIZE][BOARD_SIZE * CHAR_SIZE * 2];
    memcpy(dfs_drawing_board, drawing_board, sizeof(dfs_drawing_board));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%c", 'A' + i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            //CHAR_SIZE-1: avoid copying the end symbol of string.
            if (dfs_status.board[i][j] == 1) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    dfs_drawing_board[i][j * GAP + k] = white_piece[k];
                }
            } else if (dfs_status.board[i][j] == -1) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    dfs_drawing_board[i][j * GAP + k] = black_piece[k];
                }
            }
        }
        printf("%s\n", dfs_drawing_board[i]);
    }
}
