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

// print the board onto the terminal
void output_board(int clear) {
#ifndef DEBUG_DRAW
    if(clear)
        system("clear");
#endif
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d%s", 15 - i, i > 5 ? " " : "");

        for (int j = 0; j < BOARD_SIZE; ++j) {
            //CHAR_SIZE-1: avoid copying the end symbol of a string.
            int is_last2 = 0;
            if ((i == record[status.steps].i && j == record[status.steps].j) ||
                (i == record[status.steps - 1].i && j == record[status.steps - 1].j)) {
                is_last2 = 1;
            }
            if (status.board[i][j] == WHITE) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * CHAR_GAP + k] = white_piece[is_last2][k];
                }
            } else if (status.board[i][j] == BLACK) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * CHAR_GAP + k] = black_piece[is_last2][k];
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


    if (status.steps > 0)
        printf("%d %c%d\n", status.steps, record[status.steps].j + 'A', 15 - record[status.steps].i);

#ifdef DEBUG_DRAW
    printf("The grade estimate:\n");
    printf("WHITE: %lld\n", grade_estimate(WHITE));
    printf("BLACK: %lld\n", grade_estimate(BLACK));
#endif

}

void dfs_output_board() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d%s", 15 - i, i > 5 ? " " : "");

        for (int j = 0; j < BOARD_SIZE; ++j) {
            //CHAR_SIZE-1: avoid copying the end symbol of string.
            if (dfs_status.board[i][j] == WHITE) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * CHAR_GAP + k] = white_piece[0][k];
                }
            } else if (dfs_status.board[i][j] == BLACK) {
                for (int k = 0; k < CHAR_SIZE - 1; ++k) {
                    drawing_board[i][j * CHAR_GAP + k] = black_piece[0][k];
                }
            }
        }
        printf("%s\n", drawing_board[i]);
    }

    printf("  ");
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        printf("%c ", 'A' - 1 + i);
    }
}
