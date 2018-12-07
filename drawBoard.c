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
char white_piece[2][4] = {{"●"},
                          {"▲"}};
char black_piece[2][4] = {{"○"},
                          {"△"}};
//0==null 1==white -1==black


void output_board() {
#ifdef VERSION_COMPARE
    FILE *pout;
#ifndef USE_HASH
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d%s", 15 - i, i > 5 ? " " : "");

        for (int j = 0; j < BOARD_SIZE; ++j) {
            //CHAR_SIZE-1: avoid copying the end symbol of string.
            int is_last2 = 0;
            if ((i == record[status.steps].i && j == record[status.steps].j) ||
                (i == record[status.steps - 1].i && j == record[status.steps - 1].j)) {
                is_last2 = 1;
            }
            if (status.board[i][j] == WHITE) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = white_piece[is_last2][k];
                }
            } else if (status.board[i][j] == BLACK) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = black_piece[is_last2][k];
                }
            }
        }
        printf("%s\n", drawing_board[i]);
    }

    printf("  ");
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    pout = fopen("./_with.in","a");
#endif
    //this part is for using hash
    //if you do use hash, the output to the one that don't use hash
#ifdef USE_HASH
    pout = fopen("./_without.in", "a");
#endif

    if (pout == NULL) {
        printf("ERROR! WRONG OUTPUT FILE\n");
        exit(3);
    }

    if (dfs_status.steps > 0)
        fprintf(pout, "%d %c%d\n", status.steps, record[status.steps].j + 'A', 15 - record[status.steps].i);
    fclose(pout);
#endif

#ifndef VERSION_COMPARE
    if(dfs_status.steps>0)
        printf("%d %c%d\n", status.steps, record[status.steps].j + 'A', 15 - record[status.steps].i);
#endif

#ifndef USE_HASH
    printf("(without hash)\n");
#endif


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
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d%s", 15 - i, i > 5 ? " " : "");

        for (int j = 0; j < BOARD_SIZE; ++j) {
            //CHAR_SIZE-1: avoid copying the end symbol of string.
            int is_last2 = 0;
            if ((i == record[status.steps].i && j == record[status.steps].j) ||
                (i == record[status.steps - 1].i && j == record[status.steps - 1].j)) {
                is_last2 = 1;
            }
            if (status.board[i][j] == WHITE) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = white_piece[is_last2][k];
                }
            } else if (status.board[i][j] == BLACK) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * GAP + k] = black_piece[is_last2][k];
                }
            }
        }
        printf("%s\n", drawing_board[i]);
    }

    printf("  ");
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        printf("%c ", 'A' - 1 + i);
    }
    printf("\ncurrent step(s): %d\n", status.steps);

}
