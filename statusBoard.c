//
// Created by Nine_yota on 2018-09-29.
//

#include "statusBoard.h"

int status_board[BOARD_SIZE][BOARD_SIZE] = {};

//status_board[i][j] -> oblique_lines_1[i+j][j]
int oblique_lines_1[2 * BOARD_SIZE][BOARD_SIZE] = {};

//status_board[i][j] -> oblique_lines_2[i-j+BOARD_SIZE][j]
int oblique_lines_2[2 * BOARD_SIZE][BOARD_SIZE] = {};

/*  return -1 if that position is occupied
 *  return 0 if pc==0, which means there's nothing to do
 *  return 1 -> successful
 * */
int add_piece(int i, int j, int pc) {
    if (status_board[i][j] != 0)
        return -1;
    else if (pc == 0)
        return 0;
    status_board[i][j] = pc;
    oblique_lines_1[i + j][j] = pc;
    oblique_lines_2[i - j + BOARD_SIZE][j] = pc;
    return 1;
}